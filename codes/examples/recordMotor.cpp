// Encoder interface using STM32 encoder mode

#include "rodos.h"
#include "stm32f4xx.h"
#include <math.h>
#include "motor.h"

void encoder_init(void);
int encoder_get_count(void);
void encoder_reset_count(void);
float encoder_get_omega(const int dc);

// Motor init
Motor rw(PWM_IDX13, PWM_IDX14); // PD13 & PD14
const uint16_t duty_cycle = 30;
bool reverse_flag = true;
bool power_flag = false;

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    const uint16_t frequency = 2000;
    const uint16_t increments = 1000;

    rw.set_frequency(frequency);
    rw.set_increments(increments);

    encoder_init();
  }

  void run()
  {
    init();

    // time, power and direction
    int tasks[9][2] = {
      {0, 0}, //5s no power to make sure that it stands still
      {5, 1}, //40s one direction
      {45, 0}, //5s back to standstill
      {50, -1}, //40s other direction
      {90, 0}, //5s back to standstill
      {95, 1}, //40s one direction
      {135, -1}, //40s hard switch to other direction
      {175, 0}, //5s back to standstill
      {180, 0}, //end
    };
    int currentTask = 0;
    const int lastTask = 8;

    // Encoder init
    PRINTF("timestamp [ms], dc [count], rotations [count], degree [°], dc diff [count], w[°/s], w [rad/s]\r\n");
    int last_dc = 0;
    int dc = 0;

    const int loop_ms = 100;
    TIME_LOOP(NOW(), loop_ms * MILLISECONDS)
    {
      // Motor
      if (SECONDS_NOW() >= tasks[currentTask][0])
      {
        rw.set_duty_cycle(tasks[currentTask][1] * duty_cycle);
        //PRINTF("Started Task %d at %fs: motor speed %d\r\n", currentTask, SECONDS_NOW(), tasks[currentTask][1]);
        if (currentTask < lastTask)
        {
          currentTask++;
          //PRINTF("Now waiting for Task %d at %ds: motor speed %d\r\n", currentTask, tasks[currentTask][0], tasks[currentTask][1]);
        }
        else
        {
          PRINTF("All Tasks completed!\r\n");
          while(true){};
        }
      }

      // Encoder
      dc = encoder_get_count();
      PRINTF("%lld, %d, %d, %f, %d, %f, %f\r\n", NOW()/(MILLISECONDS), dc, dc/64,
        dc/64.f*360, dc-last_dc, (dc-last_dc)/64.f*360*loop_ms, encoder_get_omega(dc-last_dc));
      last_dc = dc;
    }
  }
} hello_encoder;

void encoder_init(void)
{
  // Encoder pins config: PA0 and PA1
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1;
  GPIOA->AFR[0] |= (1 << 0 * 4) | (1 << 1 * 4);

  // TIM2 encoder mode 3. RM0090 Rev 18, Pg. 552
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
  TIM2->CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC1P);
  TIM2->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2P);
  TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  TIM2->CCMR1 &= ~(TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
  TIM2->CCMR1 &= ~(TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);
  TIM2->CR1 |= TIM_CR1_CEN;
}

int encoder_get_count(void)
{
  return TIM2->CNT;
}

void encoder_reset_count(void)
{
  TIM2->CNT = 0;
}

float encoder_get_omega(const int dc)
{
  /*
    w = Angular velocity, rad/s
    f = Frequency, Hz
    r = Rotations in dt period
    dc = Encoder counts in dt period
    cpr = Counts per rotation
  */

  const uint8_t cpr = 64;
  const float dt = 0.100;
  const float r = dc / (float)cpr;
  const float f = r / dt;
  const float omega = 2 * M_PI * f;

  return omega;
}

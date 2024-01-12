// Motor angular rate data logger for step response
// Uses button to stop the motor

#include "rodos.h"
#include "stm32f4xx.h"
#include <math.h>
#include "motor.h"

const int loop_ms = 50;
const uint16_t duty_cycle = 100;

void encoder_init(void);
int encoder_get_count(void);
void encoder_reset_count(void);
float encoder_get_omega(const int dc);

// Motor init, PD13 & PD14
Motor rw(PWM_IDX13, PWM_IDX14);
HAL_GPIO button(GPIO_033);

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    button.init(false, 1, 0);

    const uint16_t frequency = 2000;
    const uint16_t increments = 1000;

    rw.set_frequency(frequency);
    rw.set_increments(increments);

    encoder_init();
  }

  void run()
  {
    init();

    PRINTF("t [s], w [rad/s]\n");
    TIME_LOOP(NOW(), loop_ms * MILLISECONDS)
    {
      if (button.readPins())
      {
        int dc = encoder_get_count();
        PRINTF("%f, %f\n", SECONDS_NOW(), encoder_get_omega(dc));
        rw.set_duty_cycle(duty_cycle);
      }
      else
      {
        rw.brake();
      }
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

// Counts since last call
int encoder_get_count(void)
{
  uint32_t count = TIM2->CNT;
  TIM2->CNT = 0;
  return count;
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

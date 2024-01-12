// Angular velocity of motor using encoder

#include "rodos.h"
#include "stm32f4xx.h"
#include <math.h>

void encoder_init(void);
int encoder_get_count(void);
void encoder_reset_count(void);
float encoder_get_omega(const int dc, const float dt, const uint8_t cpr);


const uint8_t cpr = 64; // Counts per revolution
const float dt = 0.015; // Sample time, s

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    encoder_init();
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), dt * 1000 * MILLISECONDS)
    {
      int dc = encoder_get_count();
      PRINTF("dc: %d, w: %f\r\n", dc, encoder_get_omega(dc, dt, cpr));
      encoder_reset_count();
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


/*
  Compute angular velocity, rad/s

  Inputs:
    dc = Encoder counts in dt period
    dt = sample time, s
    cpr = Counts per rotation

  Output:
    w = Angular velocity, rad/s
*/
float encoder_get_omega(const int dc, const float dt, const uint8_t cpr)
{
  const float r = dc / (float)cpr; // Rotations during dt
  const float f = r / dt; // Frequency, Hz
  const float w = 2 * M_PI * f;

  return w;
}

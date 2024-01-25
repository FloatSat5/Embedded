#include <math.h>

#include "stm32f4xx.h"
#include "encoder.h"

void encoder::init(void)
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

/*
  Computes angular velocity, rad/s

  Input:
    cpr = counts per revolution, s
    dt = sample time, s

  Caution: get_omega() resets TIM->CNT
*/
float encoder::get_omega(const uint16_t cpr, const float dt)
{
  int32_t dc = get_count();
  reset_count();

  const float r = dc / (float)cpr; // Rotations during dt
  const float f = r / dt;          // Frequency, Hz
  const float w = 2 * M_PI * f;    // Omega, rad/s

  return w;
}

int32_t encoder::get_count(void)
{
  return TIM2->CNT;
}

void encoder::reset_count(void)
{
  TIM2->CNT = 0;
}

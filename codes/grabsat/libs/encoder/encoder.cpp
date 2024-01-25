#include <math.h>

#include "stm32f4xx.h"
#include "encoder.h"

void encoder::init(void)
{
  // Encoder pins config: PE9 and PE11
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  GPIOE->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER11_1;
  GPIOE->AFR[1] |= (1 << (9-8) * 4) | (1 << (11-8) * 4);

  // TIM1 encoder mode 3. RM0090 Rev 19, Pg. 553
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  TIM1->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
  TIM1->CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC1P);
  TIM1->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2P);
  TIM1->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  TIM1->CCMR1 &= ~(TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
  TIM1->CCMR1 &= ~(TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);
  TIM1->CR1 |= TIM_CR1_CEN;
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
  return (short)TIM1->CNT;
}

void encoder::reset_count(void)
{
  TIM1->CNT = 0;
}

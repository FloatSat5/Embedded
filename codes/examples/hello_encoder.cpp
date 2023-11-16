// Encoder interface using STM32 encoder mode

#include "rodos.h"
#include "stm32f4xx.h"
#include <math.h>

void encoder_init(void);
int encoder_get_count(void);
void encoder_reset_count(void);
float encoder_get_omega(const int dc);

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    encoder_init();
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), 15 * MILLISECONDS)
    {
      int dc = encoder_get_count();
      PRINTF("dc: %d, w: %f\r\n", dc, encoder_get_omega(dc));
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
  const float dt = 0.015;
  const float r = dc / (float)cpr;
  const float f = r / dt;
  const float omega = 2 * M_PI * f;

  return omega;
}

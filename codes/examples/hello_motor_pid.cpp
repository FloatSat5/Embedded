// PID control of motor angular velocity

#include "pid.h"
#include "rodos.h"
#include "motor.h"
#include "stm32f4xx.h"

#include <math.h>

void encoder_init(void);
int encoder_get_count(void);
void encoder_reset_count(void);
float encoder_get_omega(const int dc);

Motor rw(PWM_IDX02, PWM_IDX03); // PE13 & PE14
const uint16_t frequency = 2000;
const uint16_t increments = 1000;

pid wpid;
const float set_point = 150;
const float kp = 0.02;
const float ki = 0.01;
const float kd = 0;
const float u_max = 100.0;
const float u_min = 0.0;

const float fc = 0.7;

// Discrete RC-LPF
class rc_lpf
{
private:
  float rc;
  float yz = 0.0, xz = 0.0;

public:
  rc_lpf()
  {

  }

  rc_lpf(const float fc)
  {
    rc = 1 / (2.0 * M_PI * fc);
  }

  void set_fc(const float fc)
  {
    rc = 1 / (2.0 * M_PI * fc);
  }

  /*           __________________
              |                  |
    X(s) ---->| 1 / (1 * RC *s ) |----> Y(s)
              |__________________|
  */
  float update(const float x, const float dt)
  {
    const float xi = 2 * rc / dt;
    const float y = (-yz * (1 - xi) + x + xz) / (1 + xi);

    yz = y;
    xz = x;

    return y;
  }
};

rc_lpf encoder_lpf(fc);

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    encoder_init();
    wpid.set_gains(kp, ki, kd);
    wpid.set_control_limits(u_min, u_max);

    rw.set_frequency(frequency);
    rw.set_increments(increments);
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), 50 * MILLISECONDS)
    {
      // Compute motor omega
      int dc = encoder_get_count();
      float omega = encoder_get_omega(dc);
      encoder_reset_count();

      // Compute control PWM
      const float error = set_point - omega;
      const float dt = 50 / 1000.0;
      const float pwm = wpid.update(error, dt);

      // Actuate motor
      rw.set_duty_cycle(pwm);

      // Display
      PRINTF("%f %f\n", set_point, omega);
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

float lw = 0;
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
  float omega = 2 * M_PI * f;

  // Simple RC filter
  omega = encoder_lpf.update(omega, 50.0 / 1000.0);

  return omega;
}

#include "motor.h"

Motor::Motor(const PWM_IDX idx_1, const PWM_IDX idx_2)
{
  pwm_1.setIdx(idx_1);
  pwm_2.setIdx(idx_2);

  pwm_1.init(frequency, increments);
  pwm_2.init(frequency, increments);
}

void Motor::brake(void)
{
  pwm_1.write(0);
  pwm_2.write(0);
}

void Motor::set_frequency(uint32_t f)
{
  pwm_1.init(f, increments);
  pwm_2.init(f, increments);
}

void Motor::set_increments(uint32_t i)
{
  pwm_1.init(frequency, i);
  pwm_2.init(frequency, i);
}

void Motor::set_duty_cycle(float duty_cycle)
{
  uint32_t pw_increments = abs(duty_cycle * (increments / 100.0f));

  if (duty_cycle < 0)
  {
    pwm_1.write(pw_increments);
    pwm_2.write(0);
  }
  else
  {
    pwm_1.write(0);
    pwm_2.write(pw_increments);
  }
}

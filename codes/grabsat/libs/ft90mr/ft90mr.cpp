#include "ft90mr.h"

ft90mr::ft90mr(const PWM_IDX pwm_idx)
{
  const uint8_t dead_bw = 20; // us
  const uint16_t time_period = 2000; // us
  const uint16_t increments = time_period / dead_bw;
  const uint16_t servo_frequency = 500;

  pwm.setIdx(pwm_idx);
  pwm.init(servo_frequency, increments);
}

/*
  Datasheet:
    Pulse width range: 1000-2000 us
    Neutral position: 1500 us
    Dead band width: <= 20 us
    CW: 1000-1500
    CCW: 1500-2000
*/
void ft90mr::run(const float speed)
{

  // abs(speed_max) -> dus: 500
  float dus = 500.0f - abs(speed) * 500.0f / 100.0f;

  const float epsilon = 1e-5;
  if (abs(speed) > epsilon)
  {
    if (speed < 0) // CCW
    {
      dus = 2000 - dus - 1;
    }
    else // CW
    {
      dus = 1000 + dus + 1;
    }
  }
  else // Stop
  {
    dus = 1500;
  }

  uint32_t increments = dus / 20;
  pwm.write(increments);
}

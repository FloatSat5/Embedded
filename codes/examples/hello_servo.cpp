#include "rodos.h"

// E9
HAL_PWM pwm_servo(PWM_IDX00);
HAL_GPIO button(GPIO_000);

const uint16_t pulse_division = 10000;
const uint16_t servo_frequency = 50;

// Servo angle to duty cycle
uint16_t get_duty_cycle(const float angle)
{
  float ms = 1 + angle / 180.0f;
  const float ms2ds = pulse_division / 20;
  return ms * ms2ds;
}

void run_servo(const float angle)
{
  uint16_t duty_cycle = get_duty_cycle(angle);
  pwm_servo.write(duty_cycle);
}

class LedPwm : public StaticThread<>
{
public:
  void init()
  {
    button.init(false, 1, 0);
    pwm_servo.init(servo_frequency, pulse_division);
  }

  void run()
  {
    init();
    while (1)
    {
      if (button.readPins())
      {
        run_servo(0);
      }
      else
      {
        // run_servo(-100);
        pwm_servo.write(180);
        pwm_servo.write(10000);
      }
    }
  }
} led_pwm;

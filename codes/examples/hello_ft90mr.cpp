// Test of FT90MR continuous rotation servo motor
// CW if user button is pressed, CCW otherwise.

/*
  Errors:
   1. Different speed for same command in opposite directions.
   2. Understanding and handling of dead bandwidth
*/

#include "rodos.h"

const float speed = 20;

HAL_PWM pwm_servo(PWM_IDX00); // E9
HAL_GPIO button(GPIO_000); // User button

void run_servo(float speed);

class HelloServo : public StaticThread<>
{
public:
  void init()
  {
    button.init(false, 1, 0);

    const uint8_t dead_bw = 20; // us
    const uint16_t time_period = 2000; // us
    const uint16_t increments = time_period / dead_bw;
    const uint16_t servo_frequency = 500;

    pwm_servo.init(servo_frequency, increments);
    run_servo(-20);
  }

  void run()
  {
    init();
    while (1)
    {
      if (button.readPins())
      {
        run_servo(40);
      }
      else
      {
        run_servo(-30);
      }
    }
  }
} hello_servo;

void run_servo(float speed)
{
  /*
    Pulse width range: 1000-2000 us
    Neutral position: 1500 us
    Dead band width: <= 20 us
    CW: 1000-1500
    CCW: 1500-2000
  */

  // abs(speed_max) -> dus: 500
  float dus = 500.0f - abs(speed) * 500.0f / 100.0f;

  const float epsilon = 1e-5;
  if (abs(speed) > epsilon) // Run
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
  uint32_t us2inc = dus / 20;
  pwm_servo.write(us2inc);
}

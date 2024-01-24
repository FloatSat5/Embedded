// Test of FT90MR continuous rotation servo motor
// CW if user button is pressed, CCW otherwise.

#include "rodos.h"
#include "ft90mr.h"

// ft90mr servo(PWM_IDX08); // TIM3 CH1 - PA6
ft90mr servo(PWM_IDX10); // TIM3 CH3 - PB0

HAL_GPIO button(GPIO_000);

class HelloServo : public StaticThread<>
{
public:
  void init()
  {
    button.init(false, 1, 0);
  }

  void run()
  {
    init();
    while (1)
    {
      if (button.readPins())
      {
        servo.run(40);
      }
      else
      {
        servo.run(-30);
      }
    }
  }
} hello_servo;

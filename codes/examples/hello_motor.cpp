// Motor driver test

#include "rodos.h"
#include "motor.h"

// PWM: PD15, Direction: PE12 & PE15
Motor rw(PWM_IDX15, GPIO_076, GPIO_079);
const float duty_cycle = 10;

class HelloMotor : public StaticThread<>
{
public:
  uint8_t duty_cycle = 0;

  void init()
  {
    rw.init();
    rw.setDutycycle(duty_cycle);
  }

  void run()
  {
    init();
  }
} hello_motor;

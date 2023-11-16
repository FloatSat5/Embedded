#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "rodos.h"

#define MOTOR_PWM_FREQUENCY 8000UL
#define MOTOR_PWM_MAX_DUTY_CYCLE 100UL

class Motor
{

public:
  Motor(PWM_IDX pwm_idx_, GPIO_PIN dir1_idx, GPIO_PIN dir2_idx);
  void init();
  void setDutycycle(float dutycycle);

private:
  HAL_PWM pwm;
  HAL_GPIO dir1;
  HAL_GPIO dir2;
};

Motor::Motor(PWM_IDX pwm_idx, GPIO_PIN dir1_pin, GPIO_PIN dir2_pin)
{
  // Instantiate PWM and GPIO objects
  pwm.setIdx(pwm_idx);
  dir1.setPinNumber(dir1_pin);
  dir2.setPinNumber(dir2_pin);
}

void Motor::init()
{
  // Init PWM
  pwm.init(8000, 100);

  // Init direction pins
  dir1.init(true, 1, 0);
  dir2.init(true, 1, 0);
}

void Motor::setDutycycle(float dutycycle)
{
  dutycycle = dutycycle * (MOTOR_PWM_MAX_DUTY_CYCLE / 100.0F);

  if (dutycycle < 0)
  {
    dutycycle = -dutycycle;
    dir1.setPins(0);
    dir2.setPins(1);
  }
  else if (dutycycle >= 0)
  {
    dir1.setPins(1);
    dir2.setPins(0);
  }
  pwm.write(dutycycle);
}

#endif // motor.h

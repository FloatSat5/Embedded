/*
  TB9051FTG interface for drive-brake (slow decay) operation.
  https://www.pololu.com/product/2997
  Rishav (2023-11-21)
*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "rodos.h"

class Motor
{
public:
  Motor(const PWM_IDX idx_1, const PWM_IDX idx_2);

  void brake(void);
  void set_frequency(uint32_t f);
  void set_increments(uint32_t i);
  void set_duty_cycle(float duty_cycle);

private:
  HAL_PWM pwm_1;
  HAL_PWM pwm_2;

  uint32_t frequency = 1500;
  uint32_t increments = 1000;
};

#endif // motor.h

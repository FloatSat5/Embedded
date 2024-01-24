/*
  FT90MR 360 degree servo driver.

  Todos:
   1. Different speed for same command in opposite directions.
   2. Understanding and handling of dead bandwidth

   Rishav (2023-01-19)
*/

#ifndef _FT90MR_H_
#define _FT90MR_H_

#include "rodos.h"

class ft90mr
{
private:
  HAL_PWM pwm;

public:
  ft90mr(const PWM_IDX pwm_idx);
  void run(const float speed);
};

#endif // ft90mr.h

// Implementation PID controller with Tustin's approximation
// Rishav (2023-12-13)

#ifndef _PID_H_
#define _PID_H_

#include <math.h>

class pid
{
private:
  float kp, ki, kd;
  float ez = 0.0, e2z = 0.0, uz = 0.0, u2z = 0.0;
  float u_min = 0.0, u_max = 0.0;

  float saturate_control(const double in);

public:
  pid();
  ~pid();

  void reset_memory(void);
  float update(const double e, const double dt);
  void set_gains(const double p, const double i, const double d);
  void set_control_limits(const double u_min, const double u_max);
};

#endif // pid.h

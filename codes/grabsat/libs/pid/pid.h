// Implementation PID controller with Tustin's approximation
// Rishav (2023-12-13)

#ifndef _PID_H_
#define _PID_H_

#include <math.h>

class pid
{
private:
  bool is_p = false, is_i = false, is_d = false; // Enable flags
  float kp, ki, kd; // PID gains
  float ez = 0.0, iz = 0.0, dz = 0.0; // Past variables
  float u_min = 0.0, u_max = 0.0; // Output limits

  float saturate_control(const float in);

public:
  pid();
  ~pid();

  void set_kp(const float p);
  void set_ki(const float i);
  void set_kd(const float d);

  void reset_memory(void);
  float update(const float e, const float dt);
  void set_control_limits(const float u_min, const float u_max);
};

#endif // pid.h

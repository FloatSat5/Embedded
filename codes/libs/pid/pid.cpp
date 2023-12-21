#include "pid.h"

pid::pid()
{
}

pid::~pid()
{
}

void pid::reset_memory(void)
{
  ez = 0.0;
  e2z = 0.0;
  u2z = 0.0;
}

inline int sign(const float x)
{
  return (x > 0.0) - (x < 0.0);
}

float pid::saturate_control(const double in)
{
  float out = in;

  if (abs(in) < u_min)
  {
    out = sign(in) * u_min;
  }
  else if (abs(in) > u_max)
  {
    out = sign(in) * u_max;
  }

  return out;
}

/*           ______________________
            |                      |
  E(s) ---->| kp + ki / s + kd * s |----> U(s)
            |______________________|
*/
float pid::update(double e, double dt)
{
  const float e_coeff = kp + 0.5 * dt * ki + 2 * kd / dt;
  const float ez_coeff = dt * ki - 4.0 * kd / dt;
  const float e2z_coeff = -kp + 0.5 * dt * ki + 2.0 * kd / dt;

  float u = u2z + e_coeff * e + ez_coeff * ez + e2z_coeff * e2z;

  ez = e;
  e2z = ez;
  u2z = u;

  return u;
}

void pid::set_gains(const double p, const double i, const double d)
{
  kp = p;
  ki = i;
  kd = d;
}

// Sign agnostic magnitude limits
void pid::set_control_limits(const double min, const double max)
{
  u_min = abs(min);
  u_max = abs(max);
}

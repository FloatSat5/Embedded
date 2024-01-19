// Discrete time RC-LPF with Tustin's approximation
// Rishav (2023-01-19)

#ifndef _RC_LPF_
#define _RC_LPF_

class rc_lpf
{
private:
  float rc;
  float yz = 0.0, xz = 0.0;

public:
  rc_lpf();
  rc_lpf(const float fc);
  void set_fc(const float fc);
  float update(const float x, const float dt);
};

#endif // rc_lpf.h

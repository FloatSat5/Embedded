// Mahony filter

#ifndef _MAHONY_H_
#define _MANONY_H_

void norm(float v[3]);

class mahony
{
private:
  bool norm_flag = false; // Normalize sensors?
  float kp = 0.0, ki = 0.0; // Feedback control gains
  float q[4] = {1.0, 0.0, 0.0, 0.0}; // Rotation quaternion
  float error_sum[3] = {0.0, 0.0, 0.0}; // Integrator memory

public:
  mahony();
  mahony(const float kp, const float ki);

  void normalize_imu(void);
  void get_ypr(float ypr[3]);
  void set_q0(const float q0[4]);
  void set_gains(const float kp, const float ki);
  void update(float a[3], float g[3], float m[3], float dt);
};

#endif // mahony.h

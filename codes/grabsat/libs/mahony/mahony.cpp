#include "mahony.h"
#include <math.h>

// To check singularity
const float epsilon = 1e-4;

// void normalize(float v[3]);
void update_quaternion(float q[3], const float g[3], const float dt);

mahony::mahony()
{
}

mahony::mahony(const float kp, const float ki)
{
  set_gains(kp, ki);
}

void mahony::set_gains(const float kp, const float ki)
{
  this->kp = kp;
  this->ki = ki;
}

void mahony::normalize_imu(void)
{
  norm_flag = true;
}

void mahony::get_ypr(float ypr[3])
{
  ypr[0] = atan2((q[1] * q[2] + q[0] * q[3]), 0.5 - (q[2] * q[2] + q[3] * q[3]));
  ypr[1] = asin(2.0 * (q[0] * q[2] - q[1] * q[3]));
  ypr[2] = atan2((q[0] * q[1] + q[2] * q[3]), 0.5 - (q[1] * q[1] + q[2] * q[2]));
}

void mahony::set_q0(const float q0[4])
{
  q[0] = q0[0];
  q[1] = q0[1];
  q[2] = q0[2];
  q[3] = q0[3];
}

void mahony::update(float a[3], float g[3], float m[3], float dt)
{
  // Normalize IMU if asked to
  if (norm_flag)
  {
    norm(a);
    norm(m);
  }

  // Intermediate variables
  const float q1q1 = q[0] * q[0];
  const float q1q2 = q[0] * q[1];
  const float q1q3 = q[0] * q[2];
  const float q1q4 = q[0] * q[3];
  const float q2q2 = q[1] * q[1];
  const float q2q3 = q[1] * q[2];
  const float q2q4 = q[1] * q[3];
  const float q3q3 = q[2] * q[2];
  const float q3q4 = q[2] * q[3];
  const float q4q4 = q[3] * q[3];

  // Magnetic field in inertial frame
  float hx = 2.0f * m[0] * (0.5f - q3q3 - q4q4) + 2.0f * m[1] * (q2q3 - q1q4) + 2.0f * m[2] * (q2q4 + q1q3);
  float hy = 2.0f * m[0] * (q2q3 + q1q4) + 2.0f * m[1] * (0.5f - q2q2 - q4q4) + 2.0f * m[2] * (q3q4 - q1q2);
  float bx = sqrt((hx * hx) + (hy * hy));
  float bz = 2.0f * m[0] * (q2q4 - q1q3) + 2.0f * m[1] * (q3q4 + q1q2) + 2.0f * m[2] * (0.5f - q2q2 - q3q3);

  // Acceleration vector estimation in body frame
  float ab[3];
  ab[0] = 2.0f * (q2q4 - q1q3);
  ab[1] = 2.0f * (q1q2 + q3q4);
  ab[2] = q1q1 - q2q2 - q3q3 + q4q4;

  // Magnetic field estimation in body frame
  float mb[3];
  mb[0] = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
  mb[1] = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
  mb[2] = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);

  // Orientation error: (a x ab) + (m x mb)
  float error[3];
  error[0] = (a[1] * ab[2] - a[2] * ab[1]) + (m[1] * mb[2] - m[2] * mb[1]);
  error[1] = (a[2] * ab[0] - a[0] * ab[2]) + (m[2] * mb[0] - m[0] * mb[2]);
  error[2] = (a[0] * ab[1] - a[1] * ab[0]) + (m[0] * mb[1] - m[1] * mb[0]);

  // Integral controller
  error_sum[0] += error[0];
  error_sum[1] += error[1];
  error_sum[2] += error[2];

  // Gyro correction using PI compensation
  g[0] += kp * error[0] + ki * error_sum[0];
  g[1] += kp * error[1] + ki * error_sum[1];
  g[2] += kp * error[2] + ki * error_sum[2];

  // Quaterion kinematics integration
  update_quaternion(q, g, dt);
}

// Unit normalize input vector
void norm(float v[3])
{
  const float norm = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] /= norm;
  v[1] /= norm;
  v[2] /= norm;
}

// Integration of quaternion kinematics
void update_quaternion(float q[3], const float g[3], const float dt)
{
  // Quaterion kinematics
  float dq[4];
  dq[0] = 0.5 * (-q[1] * g[0] - q[2] * g[1] - q[3] * g[2]);
  dq[1] = 0.5 * (q[0] * g[0] + q[2] * g[2] - q[3] * g[1]);
  dq[2] = 0.5 * (q[0] * g[1] - q[1] * g[2] + q[3] * g[0]);
  dq[3] = 0.5 * (q[0] * g[2] + q[1] * g[1] - q[2] * g[0]);

  // Integration
  q[0] += dq[0] * dt;
  q[1] += dq[1] * dt;
  q[2] += dq[2] * dt;
  q[3] += dq[3] * dt;

  // Normalization
  float norm = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
  q[0] /= norm;
  q[1] /= norm;
  q[2] /= norm;
  q[3] /= norm;
}

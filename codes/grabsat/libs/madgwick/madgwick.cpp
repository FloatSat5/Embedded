
#include <math.h>
#include "madgwick.h"

float inv_sqrt(float x);

madgwick::madgwick()
{
}

madgwick::madgwick(float beta)
{
  this->beta = beta;
}

void madgwick::set_beta(float beta)
{
  this->beta = beta;
}

// Set initial quaternion
void madgwick::set_q0(const float q0, const float q1,
                      const float q2, const float q3)
{
  this->q[0] = q0;
  this->q[1] = q1;
  this->q[2] = q2;
  this->q[3] = q3;
}

// Accelerometer + Gyroscope
void madgwick::update_imu(float gx, float gy, float gz,
                          float ax, float ay, float az, float dt)
{
  // Rate of change of quaternion from gyroscope
  float dq[4];
  dq[0] = 0.5f * (-q[1] * gx - q[2] * gy - q[3] * gz);
  dq[1] = 0.5f * (q[0] * gx + q[2] * gz - q[3] * gy);
  dq[2] = 0.5f * (q[0] * gy - q[1] * gz + q[3] * gx);
  dq[3] = 0.5f * (q[0] * gz + q[1] * gy - q[2] * gx);

  // Auxiliary variables for gradient descent
  const float _2q0 = 2.0f * q[0];
  const float _2q1 = 2.0f * q[1];
  const float _2q2 = 2.0f * q[2];
  const float _2q3 = 2.0f * q[3];
  const float _4q0 = 4.0f * q[0];
  const float _4q1 = 4.0f * q[1];
  const float _4q2 = 4.0f * q[2];
  const float _8q1 = 8.0f * q[1];
  const float _8q2 = 8.0f * q[2];
  const float q0q0 = q[0] * q[0];
  const float q1q1 = q[1] * q[1];
  const float q2q2 = q[2] * q[2];
  const float q3q3 = q[3] * q[3];

  // Gradient decent algorithm corrective step
  float dq_hat[4];
  dq_hat[0] = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
  dq_hat[1] = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q[1] - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
  dq_hat[2] = 4.0f * q0q0 * q[2] + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
  dq_hat[3] = 4.0f * q1q1 * q[3] - _2q1 * ax + 4.0f * q2q2 * q[3] - _2q2 * ay;

  // f_grad / norm(f_grad)
  const float norm = dq_hat[0] * dq_hat[0] + dq_hat[1] * dq_hat[1] + dq_hat[2] * dq_hat[2] + dq_hat[3] * dq_hat[3];
  float inv_norm = inv_sqrt(norm);

  // Estimated rate of quaternion
  dq_hat[0] *= inv_norm;
  dq_hat[1] *= inv_norm;
  dq_hat[2] *= inv_norm;
  dq_hat[3] *= inv_norm;

  // Feedback correction
  dq[0] -= beta * dq_hat[0];
  dq[1] -= beta * dq_hat[1];
  dq[2] -= beta * dq_hat[2];
  dq[3] -= beta * dq_hat[3];

  // Numerical integration
  q[0] += dq[0] * dt;
  q[1] += dq[1] * dt;
  q[2] += dq[2] * dt;
  q[3] += dq[3] * dt;

  // Normalize quaternion
  inv_norm = inv_sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
  q[0] *= inv_norm;
  q[1] *= inv_norm;
  q[2] *= inv_norm;
  q[3] *= inv_norm;
}

// Accelerometer + Gyroscope + Compass
void madgwick::update_marg(const float gx, const float gy, const float gz,
                 const float ax, const float ay, const float az,
                 const float mx, const float my, const float mz, const float dt)
{
  // Rate of change of quaternion from gyroscope
  float dq[4];
  dq[0] = 0.5f * (-q[1] * gx - q[2] * gy - q[3] * gz);
  dq[1] = 0.5f * (q[0] * gx + q[2] * gz - q[3] * gy);
  dq[2] = 0.5f * (q[0] * gy - q[1] * gz + q[3] * gx);
  dq[3] = 0.5f * (q[0] * gz + q[1] * gy - q[2] * gx);

  // Auxiliary variables for gradient descent
  const float _2q0 = 2.0f * q[0];
  const float _2q1 = 2.0f * q[1];
  const float _2q2 = 2.0f * q[2];
  const float _2q3 = 2.0f * q[3];
  const float q0q0 = q[0] * q[0];
	const float q0q1 = q[0] * q[1];
	const float q0q2 = q[0] * q[2];
	const float q0q3 = q[0] * q[3];
	const float q1q1 = q[1] * q[1];
	const float q1q2 = q[1] * q[2];
	const float q1q3 = q[1] * q[3];
	const float q2q2 = q[2] * q[2];
	const float q2q3 = q[2] * q[3];
	const float q3q3 = q[3] * q[3];
  const float _2q1mx = 2.0f * q[1] * mx;
  const float _2q0mx = 2.0f * q[0] * mx;
  const float _2q0my = 2.0f * q[0] * my;
  const float _2q0mz = 2.0f * q[0] * mz;
  const float _2q0q2 = q[0] * q[2];
  const float _2q2q3 = q[2] * q[3];

  // Reference direction of Earth's magnetic field
  const float hx = mx * q0q0 - _2q0my * q[3] + _2q0mz * q[2] + mx * q1q1 + _2q1 * my * q[2] + _2q1 * mz * q[3] - mx * q2q2 - mx * q3q3;
	const float hy = _2q0mx * q[3] + my * q0q0 - _2q0mz * q[1] + _2q1mx * q[2] - my * q1q1 + my * q2q2 + _2q2 * mz * q[3] - my * q3q3;
	const float _2bx = sqrt(hx * hx + hy * hy);
	const float _2bz = -_2q0mx * q[2] + _2q0my * q[1] + mz * q0q0 + _2q1mx * q[3] - mz * q1q1 + _2q2 * my * q[3] - mz * q2q2 + mz * q3q3;
	const float _4bx = 2.0f * _2bx;
	const float _4bz = 2.0f * _2bz;

  // Gradient decent algorithm corrective step
  float dq_hat[4];
  dq_hat[0] = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q[2] * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q[3] + _2bz * q[1]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q[2] * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	dq_hat[1] = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q[1] * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q[3] * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q[2] + _2bz * q[0]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q[3] - _4bz * q[1]) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	dq_hat[2] = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q[2] * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q[2] - _2bz * q[0]) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q[1] + _2bz * q[3]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q[0] - _4bz * q[2]) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	dq_hat[3] = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q[3] + _2bz * q[1]) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q[0] + _2bz * q[2]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q[1] * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

  // f_grad / norm(f_grad)
  const float norm = dq_hat[0] * dq_hat[0] + dq_hat[1] * dq_hat[1] + dq_hat[2] * dq_hat[2] + dq_hat[3] * dq_hat[3];
  float inv_norm = inv_sqrt(norm);

  // Estimated rate of quaternion
  dq_hat[0] *= inv_norm;
  dq_hat[1] *= inv_norm;
  dq_hat[2] *= inv_norm;
  dq_hat[3] *= inv_norm;

  // Feedback correction
  dq[0] -= beta * dq_hat[0];
  dq[1] -= beta * dq_hat[1];
  dq[2] -= beta * dq_hat[2];
  dq[3] -= beta * dq_hat[3];

  // Numerical integration
  q[0] += dq[0] * dt;
  q[1] += dq[1] * dt;
  q[2] += dq[2] * dt;
  q[3] += dq[3] * dt;

  // Normalize quaternion
  inv_norm = inv_sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
  q[0] *= inv_norm;
  q[1] *= inv_norm;
  q[2] *= inv_norm;
  q[3] *= inv_norm;
}

// rad
float madgwick::get_roll(void)
{
  return atan2(2 * (q[0] * q[1] + q[2] * q[3]),
               1 - 2 * (q[1] * q[1] + q[2] * q[2]));
}

// rad
float madgwick::get_pitch(void)
{
  return asin(2 * (q[0] * q[2] - q[3] * q[1]));
}

// rad
float madgwick::get_yaw(void)
{
  return atan2(2 * (q[0] * q[3] + q[1] * q[2]),
               1 - 2 * (q[2] * q[2] + q[3] * q[3]));
}

/*
  Fast inverse square-root
  http://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
float inv_sqrt(float x)
{
  float half_x = 0.5f * x;
  float y = x;
  long i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (1.5f - (half_x * y * y));
  return y;
}

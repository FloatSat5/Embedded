/*
  Madgwick filter implementation by Madgwick.

  Source:
      [1] Madgwick - An Efficient Orientation Filter for
          Inertial and Inertial-Magnetic sensor Arrays (2010)
      [2] https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/

  Note on quaternion notation:
    q = [q0, q1, q2, q3]' = [cos(v/2), sin(v/2) * [r1, r2, r3]']'
    q represents rotation about vector r by angle of v radians.

  Rishav (2021/08/03)
*/

#ifndef _MADGWICK_H_
#define _MADGWICK_H_

#include "math.h"

class madgwick
{
private:
public:
  float beta = 0.03;         // Filter gain
  float q[4] = {1, 0, 0, 0}; // Quaternion estimate
  float b[3] = {0, 0, 0};    // Gyro bias

  madgwick();
  madgwick(float beta);

  void set_beta(float beta);
  void set_q0(const float q0, const float q1,
              const float q2, const float q3);

  void update_imu(const float gx, const float gy, const float gz,
                  const float ax, const float ay, const float az, const float dt);

  void update_marg(const float gx, const float gy, const float gz,
                   const float ax, const float ay, const float az,
                   const float mx, const float my, const float mz, const float dt);

  float get_yaw(void);
  float get_roll(void);
  float get_pitch(void);
};

#endif // madgwick.h

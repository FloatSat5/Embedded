// Orientation estimation using Madgwick filter

#include "rodos.h"
#include "lsm9ds1.h"
#include "madgwick.h"
#include "stm32f4xx.h"

#include <math.h>

#define D2R 0.01745329251f
#define R2D 57.2957795131f

madgwick filter;

class HelloMadgwick : public StaticThread<>
{
  void init()
  {
    if (!lsm9ds1_init())
    {
      PRINTF("\nIMU error!\n");
      while (1)
      {
      }
    }

    const float beta = 1.03;
    filter.set_beta(beta);
    filter.set_q0(0, 0, 0, 1);
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), 15 * MILLISECONDS)
    {
      float a[3], g[3], m[3];

      if (!(lsm9ds1_read_accel(a) && lsm9ds1_read_gyro(g) && lsm9ds1_read_mag(m)))
      {
        while (1)
        {
        }
      }

      float norm = 1.0 / sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
      a[0] = a[0] * norm;
      a[1] = a[1] * norm;
      a[2] = a[2] * norm;

      norm = 1.0 / sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
      m[0] = m[0] * norm;
      m[1] = m[1] * norm;
      m[2] = m[2] * norm;

      float dt = 15.0 / 1000.0;
      // filter.update_marg(g[0] * D2R, g[1] * D2R, g[2] * D2R, a[0], a[1], a[2], m[0], m[1], m[2], dt);
      filter.update_marg(g[0] * D2R, g[1] * D2R, g[2] * D2R, a[0], a[1], a[2], m[0], m[1], m[2], dt);

      float ypr[3];
      ypr[0] = filter.get_yaw();
      ypr[2] = filter.get_pitch();
      ypr[1] = filter.get_roll();
      PRINTF("%f, %f, %f\n", ypr[0] * R2D, ypr[1] * R2D, ypr[2] * R2D);
    }
  }
} hello_madgwick;

// Orientation estimation using Madgwick filter

#include "rodos.h"
#include "mahony.h"
#include "lsm9ds1.h"
#include "lanczos.h"
#include "stm32f4xx.h"

#include <math.h>

#define D2R 0.01745329251f
#define R2D 57.2957795131f

const float kp = 50;
const float ki = 0;
mahony filter(kp, ki);

const float b[3] = {38.392299, 12.741417, 46.423205};
const float Ainv[3][3] = {{-0.001353, 0.004559, 0.032164},
                          {0.017233, 0.020416, -0.002169},
                          {-0.019714, 0.016308, -0.003140}};

HAL_UART bluetooth(UART_IDX4);

class HelloMahony : public StaticThread<>
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

    bluetooth.init(115200);
    filter.normalize_imu();
  }

  float get_yaw(float m[3], float ypr[3])
  {
    lnz::Euler ea321({0, ypr[1], ypr[2]});
    lnz::Vector<3> mv({m[0], m[1], m[2]});
    mv = trans(ea321.get_dcm()) * mv;

    float yaw = atan2(-mv(1), mv(0));
    if (yaw < 0)
    {
      yaw += 2 * M_PI;
    }

    return yaw;
  }

  void run()
  {
    TIME_LOOP(NOW(), 15 * MILLISECONDS)
    {
      float a[3], g[3], m[3];

      if (!(lsm9ds1_read_accel(a) && lsm9ds1_read_gyro(g) && lsm9ds1_read_mag(m)))
      {
        while (1)
        {
        }
      }

      // magcal(m, Ainv, b);

      g[0] = g[0] * D2R * 0.1;
      g[1] = g[1] * D2R * 0.1;
      g[2] = g[2] * D2R * 0.1;

      g[0] = -g[0];
      a[0] = -a[0];

      float dt = 15.0 / 1000.0;
      filter.update(a, g, m, dt);

      float ypr[3];
      filter.get_ypr(ypr);

      float yaw = get_yaw(m, ypr);

      // PRINTF("%f, %f, %f\n", ypr[0] * R2D, ypr[1] * R2D, ypr[2] * R2D);
      // PRINTF("%f, %f, %f\n", m[0], m[1], m[2]);

      char msg[50];
      PRINTF("%f, %f, %f\n", yaw * R2D, ypr[1] * R2D, ypr[2] * R2D);
      int len = SNPRINTF(msg, sizeof(msg), "%f, %f, %f\n", yaw * R2D, ypr[1] * R2D, ypr[2] * R2D);
      bluetooth.write(msg, len);
    }
  }
} hello_madgwick;

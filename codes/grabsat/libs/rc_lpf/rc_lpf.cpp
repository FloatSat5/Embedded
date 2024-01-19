#include <math.h>
#include "rc_lpf.h"

rc_lpf::rc_lpf()
{
}

rc_lpf::rc_lpf(const float fc)
{
  rc = 1 / (2.0 * M_PI * fc);
}

void rc_lpf::set_fc(const float fc)
{
  rc = 1 / (2.0 * M_PI * fc);
}

/*           __________________
            |                  |
  X(s) ---->| 1 / (1 * RC *s ) |----> Y(s)
            |__________________|
*/
float rc_lpf::update(const float x, const float dt)
{
  const float xi = 2 * rc / dt;
  const float y = (-yz * (1 - xi) + x + xz) / (1 + xi);

  yz = y;
  xz = x;

  return y;
}

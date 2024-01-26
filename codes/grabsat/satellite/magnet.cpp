#include "rodos.h"
#include "magnet.h"

// false = off
bool magnet::state = false;

HAL_GPIO mag_pin(GPIO_064); // PE0

void magnet::init(void)
{
  mag_pin.init(true, 1, 0);
}

void magnet::actuate(const bool on_off)
{
  magnet::state = on_off;
  mag_pin.setPins(on_off);
}

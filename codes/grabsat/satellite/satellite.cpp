#include "magnet.h"
#include "satellite.h"
#include "satellite_config.h"

#include <rodos.h>
#include <inttypes.h>

// Modes
const char *modes[] = {"idle", "motor", "yaw", "omega", "debris"};
satellite_mode current_mode = idle;

void satellite::init(void)
{
  magnet::init();
}

// Print the current satellite mode
void satellite::print_mode()
{
  PRINTF("Mode: %s\n", modes[int(current_mode)]);
}

#include "satellite_config.h"
#include "satellite.h"
#include "ft90mr.h"

#include <rodos.h>
#include <inttypes.h>

// Modes
const char *modes[] = {"idle", "motor", "yaw", "omega", "debris"};
satellite_mode current_mode = idle;

// Servo motors
ft90mr arm(SERVO_ARM_PWM_IDX);

void satellite::init(void)
{
}

// Print the current satellite mode
void satellite::print_mode()
{
  PRINTF("Mode: %s\n", modes[int(current_mode)]);
}

void satellite::extend_arm(const float speed)
{
  arm.run(SERVO_ARM_SPEED);
}

void satellite::retract_arm(const float speed)
{
  arm.run(-SERVO_ARM_SPEED);
}

void satellite::stop_arm(void)
{
  arm.run(0);
}

void satellite::actuate_magnet(const bool state)
{
}

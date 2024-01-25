#include "switch_board.h"
#include "motor_control.h"

void switch_board::stop_motor_control(void)
{
  motor_control_thread.stop_flag = true;
}

void switch_board::resume_motor_control(void)
{
  if (motor_control_thread.stop_flag == true)
  {
    motor_control_thread.stop_flag = false;
    motor_control_thread.resume();
  }
}

// Enable input threads while disabling the unwanted
void switch_board::enable_thread(const satellite_mode m)
{
  switch (m)
  {
  case idle:
  {
    stop_motor_control();
    break;
  }

  case motor:
  {
    resume_motor_control();
    break;
  }

  case yaw:
  {
    stop_motor_control();
    break;
  }

  default:
    break;
  }
}

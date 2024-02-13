#include "arm.h"
#include "control.h"
#include "switch_board.h"

void switch_board::stop_arm(void)
{
  arm_thread.stop_flag = true;
}

void switch_board::resume_arm(void)
{
  if (arm_thread.stop_flag == true)
  {
    arm_thread.stop_flag = false;
    arm_thread.resume();
  }
}

void switch_board::stop_control(void)
{
  control_thread.stop_flag = true;
}

void switch_board::resume_control(void)
{
  if (control_thread.stop_flag == true)
  {
    control_thread.stop_flag = false;
  }
}

// Enable input threads while disabling the unwanted
void switch_board::enable_thread(const satellite_mode m)
{
  switch (m)
  {
  case idle:
  {
    stop_control();
    break;
  }

  case motor:
  {
    resume_control();
    break;
  }

  case yaw:
  {
    resume_control();
    break;
  }

  case omega:
  {
    resume_control();
    break;
  }

  case debris:
  {
    break;
  }

  default:
    break;
  }
}

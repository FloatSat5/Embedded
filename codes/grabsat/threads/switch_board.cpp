#include "switch_board.h"
#include "control.h"

void switch_board::stop_control(void)
{
  control_thread.stop_flag = true;
}

void switch_board::resume_control(void)
{
  if (control_thread.stop_flag == true)
  {
    control_thread.stop_flag = false;
    control_thread.resume();
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
    stop_control();
    break;
  }

  default:
    break;
  }
}

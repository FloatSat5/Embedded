// Interface to disable and continue threads
// Rishav (2023-01-25)

#ifndef _SWITCH_BOARD_H_
#define _SWITCH_BOARD_H_

#include "satellite.h"

namespace switch_board
{
  void stop_motor_control(void);
  void resume_motor_control(void);
  void enable_thread(const satellite_mode m);
}

#endif // switch_board.h

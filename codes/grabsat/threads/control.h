// Motor angular rate control thread
// Rishav (2023-01-25)

#ifndef _CONTROL_THREAD_H_
#define _CONTROL_THREAD_H_

#include "rodos.h"

class ControlThread : public StaticThread<>
{
public:
  bool stop_flag = true;

  void run();
  void init();
};

static ControlThread control_thread;

#endif // control.h

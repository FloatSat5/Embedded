// Motor angular rate control thread
// Rishav (2023-01-25)

#ifndef _MOTOR_CONTROL_THREAD_H_
#define _MOTOR_CONTROL_THREAD_H_

#include "rodos.h"

class MotorControlThread : public StaticThread<>
{
public:
  bool stop_flag = true;

  void run();
  void init();
};

static MotorControlThread motor_control_thread;

#endif // motor_control.h

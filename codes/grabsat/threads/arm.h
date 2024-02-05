#ifndef _ARM_THREAD_H_
#define _ARM_THREAD_H_

#include "rodos.h"

class ArmThread : public StaticThread<>
{
public:
  bool stop_flag = true;
  bool dir_flag = true; // true : extend arm, false: retract
  int period = 0;       // Actuation period, ms

  void run();
  void init();
  void check_limits();
};

extern ArmThread arm_thread;

#endif // arm.h

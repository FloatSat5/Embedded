// Communication thread with groundstation
// Rishav (2023-01-25)

#ifndef _GROUNDSTATION_THREAD_H_
#define _GROUNDSTATION_THREAD_H_

#include "rodos.h"

class GroundstationThread : public StaticThread<>
{
  void run();
  void init();
};

static GroundstationThread groundstation_thread;

#endif // groundstation.h

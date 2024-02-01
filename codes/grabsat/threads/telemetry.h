#ifndef _TELEMETRY_THREAD_H_
#define _TELEMETRY_THREAD_H_

#include "rodos.h"

class TelemetryThread : public StaticThread<>
{
  int flag = 1;
  void run();
  void init();
};

static TelemetryThread telemetry_thread;

#endif // telemetry.h

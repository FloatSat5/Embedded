#ifndef _TOPICS_H_
#define _TOPICS_H_

#include "rodos.h"

struct telemetry_struct
{
  float g[3];   // Gyro readings, deg/s
  float ypr[3]; // {yaw, pitch, roll}, deg
  float w;      // Motor angular velocity, deg/s
  float v;      // Battery voltage, V
};

extern Topic<telemetry_struct> telemetry_topic;

#endif // topics.h

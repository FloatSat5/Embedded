#ifndef _GRABSAT_TELEMETRY_H_
#define _GRABSAT_TELEMETRY_H_

// Order must correspond to telemetry_id[]
enum telemetry_idx
{
  motav, // Motor angular velocity, deg/s [float]
  angve, // Angular velocity (x, y, z), deg/s [float]
  angpo, // Angular position (roll, pitch, yaw), deg [float]
  armpo, // Arm position [bool]
  batvo, // Battery voltage, V [float]
  elcur, // Electric current, mA [float]
  magst  // Electromagnet status [bool]
};

struct telemetry_frame_t
{
 const char* id;
 const int n;
}extern telemetry_frame;

namespace telemetry
{
  void transmit(void);
}

#endif // telemetry.h

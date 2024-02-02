// Satellite functions that does not go on any library

#ifndef _GRABSAT_SATELLITE_H_
#define _GRABSAT_SATELLITE_H_

enum satellite_mode
{
  idle,   // Do nothing at all
  motor,  // Control motor angular rate
  yaw,    // Control satellite heading
  omega,  // Control satellite omega
  debris  // Perform GrabSat mission
};

extern satellite_mode current_mode;

namespace satellite
{
  void init(void);
  void print_mode(void);
};

#endif // satellite.h

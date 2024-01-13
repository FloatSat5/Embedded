// Rishav (2024-01-12)

#ifndef _GRABSAT_TELECOMMAND_H_
#define _GRABSAT_TELECOMMAND_H_

#include "rodos.h"
#include <inttypes.h>

// Order must correspond to telecommands[]
enum teleidx
{
  // Modes
  mosav, // Set motor angular velocity, deg/min [float]
  sangp, // Satellite angular position angle, degree [float]
  sangv, // Satellite angular velocity, deg/min [float]
  swoff, // Switch off [bool]

  // GrabSat mission
  fideb, // Find the debris [bool]
  exarm, // Extend arm [bool] [? float]
  semag, // Set electromagnet active/inactive [bool]
  rearm, // Retract arm [bool]

  // Control params
  gkpmw,   // Set Kp for motor angular velocity [float]
  gkimw,   // Set Ki for motor angular velocity [float]
  gkpsa,   // Set Kp for sat angle [float]
  gkisa,   // Set Ki for sat angle [float]
  gkpsw,   // Set Kp for sat angular velocity [float]
  gkisw,   // Set Ki for sat angular velocity [float]

  // None of above
  invalid  // Invalid telecommand
};

struct telecommands_t
{
  const char *command;
  float value;
}extern telecommands[];

namespace telecommand
{
  teleidx parse(const uint8_t *msg, int n);
  void execute(void);

  void print(void);
  void print(teleidx ti);
};

#endif // telecommand.h

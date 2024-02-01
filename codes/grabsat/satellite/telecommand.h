// Rishav (2024-01-12)

#ifndef _GRABSAT_TELECOMMAND_H_
#define _GRABSAT_TELECOMMAND_H_

#include "rodos.h"
#include "satellite_config.h"
#include <inttypes.h>

extern HAL_UART bluetooth;
extern char msg[BLUETOOTH_BUFFER_LEN];

// Order must correspond to telecommands[]
enum telecommand_idx
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

  // Control paramsexarm
  gkpmw, // Set Kp for motor angular velocity [float]
  gkimw, // Set Ki for motor angular velocity [float]
  gkpsa, // Set Kp for sat angle [float]
  gkisa, // Set Ki for sat angle [float]
  gkpsw, // Set Kp for sat angular velocity [float]
  gkisw, // Set Ki for sat angular velocity [float]

  // None of above
  invalid // Invalid telecommand
};

struct telecommands_t
{
  const char *command;
  float value;
}extern telecommands[];

namespace telecommand
{
  telecommand_idx parse(const uint8_t *msg, int n);
  void execute(const telecommand_idx idx);

  bool print(void);
  bool print(const telecommand_idx ti);
};

#endif // telecommand.h

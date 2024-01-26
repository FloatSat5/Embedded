#include "satellite.h"
#include "multimeter.h"
#include "telecommand.h"
#include "switch_board.h"
#include "satellite_config.h"

#include <stdio.h>

#define MAX_KEY_LENGTH 6

// Did you add telecommand_idx for new telecommand?
telecommands_t telecommands[] =
{
  {"mosav", 0.0},
  {"sangp", 0.0},
  {"sangv", 0.0},
  {"swoff", 0.0},
  {"fideb", 0.0},
  {"exarm", 0.0},
  {"semag", 0.0},
  {"rearm", 0.0},
  {"gkpmw", PID_MOTOR_KP},
  {"gkimw", PID_MOTOR_KI},
  {"gkpsa", PID_YAW_KP},
  {"gkisa", PID_YAW_KI},
  {"gkpsw", PID_OMEGA_KP},
  {"gkisw", PID_OMEGA_KI}
};

// Returns telecommand_idx for valid telecommand and -1 otherwise
telecommand_idx decode_telecommand(const char *tc, const float value)
{
  for (int i = 0; i < (int)(sizeof(telecommands) / sizeof(telecommands_t)); i++)
  {
    if (!strcmp(telecommands[i].command, tc))
    {
      telecommands[i].value = value;
      return (telecommand_idx)i;
    }
  }

  return invalid;
}

telecommand_idx telecommand::parse(const uint8_t *msg, int n)
{

  int index = 0;
  while (index < n)
  {
    char command[MAX_KEY_LENGTH];
    float value = 0;

    // Extract command
    int i = 0;
    while ((msg[index] != ',') && (index < n) && (i < MAX_KEY_LENGTH - 1))
    {
      command[i++] = msg[index++];
    }
    command[i] = '\0';

    // Missing comma or no value after comma
    if (index + 1 >= n || msg[index] != ',')
    {
      return invalid;
    }

    // Extract the value of command
    int flag = sscanf((char *)&msg[index + 1], "%f", &value);

    // String is not valid float
    if (!flag)
    {
      return invalid;
    }

    return decode_telecommand(command, value);
  }

  return invalid;
}

void telecommand::execute(const telecommand_idx idx)
{

  switch (idx)
  {
  case mosav:
  {
    current_mode = satellite_mode::motor;
    break;
  }

  case sangp:
  {
    current_mode = satellite_mode::yaw;
    break;
  }

  case sangv:
  {
    current_mode = satellite_mode::omega;
    break;
  }

  case exarm:
  {
    satellite::extend_arm(SERVO_ARM_SPEED);
    break;
  }

  case swoff:
  {
    current_mode = satellite_mode::idle;
    break;
  }

  default:
    break;
  }

  switch_board::enable_thread(current_mode);
}

// Print the current status of all telecommands
bool telecommand::print(void)
{
  PRINTF("\n");
  for (int i = 0; i < (int)(sizeof(telecommands) / sizeof(telecommands_t)); i++)
  {
    PRINTF("%s: %f\n", telecommands[i].command, telecommands[i].value);
  }
  return true;
}

// Display value of specific telecommand
bool telecommand::print(const telecommand_idx ti)
{
  bool output = false;

  if (ti == telecommand_idx::invalid)
  {
    PRINTF("Invalid command!\n");
    output = false;
  }
  else
  {
    PRINTF("%s: %f\n", telecommands[ti].command, telecommands[ti].value);
    output = true;
  }

  return output;
}

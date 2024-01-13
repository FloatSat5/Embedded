#include "telecommand.h"
#include <stdio.h>

#define MAX_KEY_LENGTH 6
#define TELECOMMAND_INVALID 555

// Did you add teleidx for new telecommand?
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
  {"gkpmw", 0.0},
  {"gkimw", 0.0},
  {"gkpsa", 0.0},
  {"gkisa", 0.0},
  {"gkpsw", 0.0},
  {"gkisw", 0.0}
};

// Returns teleidx for valid telecommand and -1 otherwise
teleidx decode_telecommand(const char *tc, const float value)
{
  for (int i = 0; i < (int)(sizeof(telecommands) / sizeof(telecommands_t)); i++)
  {
    if (!strcmp(telecommands[i].command, tc))
    {
      telecommands[i].value = value;
      return (teleidx)i;
    }
  }

  return invalid;
}

teleidx telecommand::parse(const uint8_t *msg, int n)
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

    // If no value
    if (index + 1 >= n)
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

// Print the current status of all telecommands
void telecommand::print(void)
{
  PRINTF("\n");
  for (int i = 0; i < (int)(sizeof(telecommands) / sizeof(telecommands_t)); i++)
  {
    PRINTF("%s: %f\n", telecommands[i].command, telecommands[i].value);
  }
}

// Display value of specific telecommand
void telecommand::print(teleidx ti)
{
  if (ti == teleidx::invalid)
  {
    PRINTF("Invalid command!\n");
  }
  else
  {
    PRINTF("%s: %f\n", telecommands[ti].command, telecommands[ti].value);
  }
}

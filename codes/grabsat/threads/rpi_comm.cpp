#include "utils.h"
#include "rpi_comm.h"

#include <stdio.h>

#define MAX_IDX_LENGTH 6

HAL_UART rpi_uart(RPI_COMM_UART_IDX);
rpi_data_t rpi_data;

void RpiCommThread::init()
{
  rpi_uart.init(RPI_COMM_UART_BAUD);
}

void RpiCommThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      suspendCallerUntil(END_OF_TIME);
    }

    uint8_t input_msg[50];
    uint8_t msg_size = rpi_uart.read(input_msg, 15);
    input_msg[msg_size] = '\0';

    if (msg_size)
    {
      bool status = parse(input_msg, msg_size);
      print(status);
    }

    suspendCallerUntil(NOW() + PERIOD_RPI_COMM * MILLISECONDS);
  }
}

bool RpiCommThread::parse(const uint8_t *msg, int n)
{

  int index = 0;
  while (index < n)
  {
    char command[MAX_IDX_LENGTH];
    float value = 0;

    // Extract command
    int i = 0;
    while ((msg[index] != ',') && (index < n) && (i < MAX_IDX_LENGTH - 1))
    {
      command[i++] = msg[index++];
    }
    command[i] = '\0';

    // Missing comma or no value after comma
    if (index + 1 >= n || msg[index] != ',')
    {
      return false;
    }

    // Extract the value of command
    int flag = sscanf((char *)&msg[index + 1], "%f", &value);

    // String is not valid float
    if (!flag)
    {
      return false;
    }

    return populate_rpi_data(command, value);
  }

  return false;
}

// Polulates rpi_data for valid idx and returns false for invalid idx
bool RpiCommThread::populate_rpi_data(const char *idx, const float value)
{
  bool output = true;

  if(!strcmp("dang", idx))
  {
    rpi_data.del = value;
  }
  else if(!strcmp("dist", idx))
  {
    rpi_data.dis = value;
  }
  else if(!strcmp("scam", idx))
  {
    rpi_data.psi = value;
  }
  else
  {
    output = false;
  }

  return output;
}

void RpiCommThread::print(bool parse_status)
{
  if (parse_status)
  {
    PRINTF("del: %f\370, dis: %f cm, psi: %d\370\n", rpi_data.del, rpi_data.dis, rpi_data.psi);
  }
  else
  {
    PRINTF("Invalid RPi data format!\n");
  }
}

RpiCommThread rpi_command_thread;

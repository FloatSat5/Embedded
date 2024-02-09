#include "utils.h"
#include "rpi_comm.h"
#include "telecommand.h"

#include <stdio.h>

#define MAX_IDX_LENGTH 6
#define INPUT_LENGTH_OFFSET 0

HAL_UART rpi_uart(RPI_COMM_UART_IDX);
rpi_data_t temp_rpi_data;
rpi_data_t rpi_data;

void RpiCommThread::init()
{
  rpi_uart.init(RPI_COMM_UART_BAUD);
}

void RpiCommThread::run()
{
  init();

  while (1)
  {
    if (stop_flag)
    {
      suspendCallerUntil(END_OF_TIME);
    }

    uint8_t input_msg[100];
    uint8_t msg_size = rpi_uart.read(input_msg, sizeof(input_msg));
    input_msg[msg_size] = '\0';

    if (msg_size)
    {
      PRINTF("\n%s\n", input_msg);
      bool status = parse(input_msg, msg_size);
      print(status);
    }

    suspendCallerUntil(NOW() + PERIOD_RPI_COMM * MILLISECONDS);
  }
}

void RpiCommThread::transmit(rpi_mode rm)
{
  // Checksum is always 11
  char rpi_msg[12] = "mode,_,x,11";

  switch (rm)
  {
  case DEBRIS:
    rpi_msg[5] = '1';
    break;

  case STAR_TRACKER:
    rpi_msg[5] = '2';
    break;

  case IDLE:
    rpi_msg[5] = '3';
    break;

  default:
    break;
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
    index = index + 2;

    while ((msg[index] != ',') && (index < n))
    {
      index++;
    }
    index++;

    // String is not valid float
    if (!flag)
    {
      return false;
    }

    populate_rpi_data(command, value);
  }

  if (temp_rpi_data.x == n - INPUT_LENGTH_OFFSET)
  {
    rpi_data = temp_rpi_data;
    return true;
  }

  return false;
}

// Polulates rpi_data for valid idx and returns false for invalid idx
bool RpiCommThread::populate_rpi_data(const char *idx, const float value)
{
  bool output = true;

  if (!strcmp("dang", idx))
  {
    temp_rpi_data.del = value;
  }
  else if (!strcmp("dist", idx))
  {
    temp_rpi_data.dis = value;
  }
  else if (!strcmp("scam", idx))
  {
    temp_rpi_data.psi = value;
  }
  else if (!strcmp("x", idx))
  {
    temp_rpi_data.x = value;
  }
  else
  {
    output = false;
  }

  return output;
}

void RpiCommThread::print(bool parse_status)
{
  char send[100];
  int len;

  if (parse_status)
  {
    PRINTF("del: %f, dis: %f, psi: %d, x: %d\n", rpi_data.del, rpi_data.dis, rpi_data.psi, rpi_data.x);
    len = SNPRINTF(send, sizeof(send), "del: %f, dis: %f, psi: %d, x: %d\n", rpi_data.del, rpi_data.dis, rpi_data.psi, rpi_data.x);
  }
  else
  {
    PRINTF("Valid RIP data format!\n");
    len = SNPRINTF(send, sizeof(send), "Valid RIP data format!\n");
  }

  bluetooth.write(send, len);
}

RpiCommThread rpi_command_thread;

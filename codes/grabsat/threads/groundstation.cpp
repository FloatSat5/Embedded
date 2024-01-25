#include "satellite_config.h"
#include "groundstation.h"
#include "telecommand.h"
#include "satellite.h"

HAL_UART bluetooth(GROUNDSTATION_UART_IDX);

void GroundstationThread::run()
{
  while (1)
  {
    uint8_t input_msg[50];
    uint8_t msg_size = bluetooth.read(input_msg, 15);
    input_msg[msg_size] = '\0';

    if (msg_size)
    {
      satellite_mode temp_mode = current_mode;

      telecommand_idx idx = telecommand::parse(input_msg, msg_size);
      telecommand::execute(idx);

      if (telecommand::print((telecommand_idx)idx) && temp_mode != current_mode)
      {
        satellite::print_mode();
      }
    }

    suspendCallerUntil(NOW() + PERIOD_GROUNDSTATION * MILLISECONDS);
  }
}

void GroundstationThread::init()
{
  bluetooth.init(GROUNDSTATION_UART_BAUD);
}

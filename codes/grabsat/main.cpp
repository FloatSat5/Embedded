#include "rodos.h"
#include "hal_uart.cpp"
#include "telecommand.h"

HAL_UART bluetooth(UART_IDX3);

class HelloBlinky : public StaticThread<>
{
  void init()
  {
  }

  void run()
  {
    TIME_LOOP(NOW(), 10 * MILLISECONDS)
    {
      uint8_t input_msg[50];
      uint8_t msg_size = bluetooth.read(input_msg, 15);
      input_msg[msg_size] = '\0';

      if (msg_size)
      {
        teleidx idx = telecommand::parse(input_msg, msg_size);
        telecommand::print((teleidx)idx);
      }
    }
  }
} hello_blinky;

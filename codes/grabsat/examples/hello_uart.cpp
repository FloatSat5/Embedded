// Using UART other than UART_IDX3 used for debug
// Sends msg every second and echos whatever it receives

#include "rodos.h"
#include "hal_uart.cpp"

uint32_t baudrate = 115200;

// IDX3 is default UART used by RODOS
// HAL_UART pi(UART_IDX4); // tx: PC10, rx: PC11
HAL_UART pi(UART_IDX3); // tx: PD8, rx: PD9

uint8_t msg[] = "Hi I am STM!\n";
uint16_t counter;

class Hello_Uart : public StaticThread<>
{
  void init()
  {
    pi.init(baudrate);
  }

  void run()
  {
    void init();

    TIME_LOOP(NOW(), 10 * MILLISECONDS)
    {

      uint8_t input[50];
      uint8_t msg_size = pi.read(input, sizeof(input));
      input[msg_size] = '\0';

      // If input then echo
      if(msg_size)
      {
        pi.write(input, msg_size);
        pi.write("\n", 2);
      }

      if(counter >= 100)
      {
        pi.write(msg, sizeof(msg));
        counter = 0;
      }
      counter++;
    }
  }
} hello_uart;

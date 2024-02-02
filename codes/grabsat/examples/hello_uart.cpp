// Using UART other than UART_IDX3 used for debug
// Sends msg every second and echos whatever it receives

#include "rodos.h"
#include "hal_uart.cpp"

uint32_t baudrate = 115200;

// IDX3 is default UART used by RODOS
HAL_UART pi(UART_IDX4); // tx: PC10, rx: PC11
//HAL_UART pi(UART_IDX3); // tx: PD8, rx: PD9

char msg[] = "Hello";
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

    TIME_LOOP(NOW(), 50 * MILLISECONDS)
    {
      uint8_t input[15];
      uint8_t msg_size = pi.read(input, sizeof(input));
      input[msg_size] = '\0';

      // Read out input msg
      if(msg_size)
      {
        PRINTF("got message :");
        for(uint8_t i = 0; i <= msg_size ; i++){
          PRINTF("%c", input[i]);
        }
        PRINTF("\n");
      }

      pi.write(msg, sizeof(msg));

    }
  }
} hello_uart;

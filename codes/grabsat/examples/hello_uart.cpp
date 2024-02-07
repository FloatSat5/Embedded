// Using UART other than UART_IDX3 used for debug
// Sends msg every second and echos whatever it receives

#include "rodos.h"
#include "hal_uart.cpp"
// use to check if array element is a int
#include <ctype.h>

uint32_t baudrate = 115200;

// IDX3 is default UART used by RODOS
HAL_UART pi(UART_IDX6); // tx: PC10, rx: PC11
// HAL_UART pi(UART_IDX3); // tx: PD8, rx: PD9

int star_camera_angle = 0;
float angle_to_debris = 0;
float distance_to_debris = 0;

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
      uint8_t pi_msg[50];
      uint8_t msg_size = pi.read(pi_msg, sizeof(pi_msg)); // -> should look like (dang122.444dist13.2412412scam180x20)

      int checksumPosition = 0;
      // parser to get the checksum
      for (int i = msg_size - 1; i <= 1; i--)
      {
        // get the checksum caracter and get position in array
        if (pi_msg[i] == 'x')
        {
          break;
          checksumPosition = i;
          PRINTF("checksum position is = %d\n", checksumPosition);
        }
      }

      int checksum = 0;

      // get the checksum
      for (int i = checksumPosition + 1; i <= msg_size; i++)
      {
        checksum *= 10;
        checksum += (int)pi_msg[i];
      }

      PRINTF("checksum = %d\n", checksum);

      // init variable befor the loop
      bool decimal = false;
      float decimal_multiplier = 0.1;
      int end_of_angle = 0;

      // if checksum has the rigth length read message else output error
      if (checksum == msg_size - 1)
      {
        // get angle to debris (read message until next char element, get the dot if float)
        for (int i = 3; !isdigit(pi_msg[i]) && pi_msg[i] == '.'; i++)
        {
          // get the moment where it is decimal
          if (pi_msg[i] == '.')
          {
            decimal = true;
          }

          // if number not decimal multiply by 10, else the input
          if (!decimal)
          {
            angle_to_debris *= 10;
            angle_to_debris += (int)pi_msg[i];
          }
          else
          {
            angle_to_debris = (int)pi_msg[i] * decimal_multiplier;
            decimal_multiplier *= 0.1;
          }
          // get postion of last element
          end_of_angle = i;
        }

        PRINTF("angle to debris = %f\n", angle_to_debris);
        
        //reset values
        decimal = false;
        decimal_multiplier = 0.1;

        // get the distance to the debris TODO
        for (int i = end_of_angle + 4; !isdigit(pi_msg[i]) && pi_msg[i] == '.'; i++)
        {
          // get the moment where it is decimal
          if (pi_msg[i] == '.')
          {
            decimal = true;
          }

          // if number not decimal multiply by 10, else the input
          if (!decimal)
          {
            distance_to_debris *= 10;
            distance_to_debris += (int)pi_msg[i];
          }
          else
          {
            distance_to_debris = (int)pi_msg[i] * decimal_multiplier;
            decimal_multiplier *= 0.1;
          }
          // get postion of last element
          end_of_angle = i;
        }

        PRINTF("distance to debris = %f\n", distance_to_debris);

        // angle of the star traker
        for (int i = end_of_angle + 4; !isdigit(pi_msg[i]); i++)
        {
          star_camera_angle *= 10;
          star_camera_angle += (int)pi_msg[i];
        }

        PRINTF("distance to debris = %d\n", star_camera_angle);

        //add end of array
        pi_msg[msg_size] = '\0';

        // Read out pi_msg msg -> should look like (dang122.444dist13.2412412scam180x20)
        if (msg_size)
        {
          PRINTF("got message :");
          for (uint8_t i = 0; i <= msg_size; i++)
          {
            PRINTF("%c", pi_msg[i]);
          }
          PRINTF("\n");
        }

      }
      else
      {
        PRINTF("recieved message is");
      }

      pi.write(msg, sizeof(msg));
    }
  }
} hello_uart;

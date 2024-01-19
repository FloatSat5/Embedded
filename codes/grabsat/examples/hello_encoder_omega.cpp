// Angular velocity of motor using encoder

#include "rodos.h"
#include "stm32f4xx.h"
#include "encoder.h"

const uint16_t motor_cpr = 64; // Counts per rotation
const uint8_t dt = 15; // Milis

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    encoder::init();
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), dt * MILLISECONDS)
    {
      PRINTF("count: %f\r\n", encoder::get_omega(motor_cpr, dt/1000.0));
    }
  }
} hello_encoder;

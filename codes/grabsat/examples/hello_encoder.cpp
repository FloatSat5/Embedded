// Encoder interface using STM32 encoder mode

#include "rodos.h"
#include "stm32f4xx.h"
#include "encoder.h"

class HelloEncoder : public StaticThread<>
{
  void init()
  {
    encoder::init();
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), 15 * MILLISECONDS)
    {
      PRINTF("count: %ld\r\n", encoder::get_count());
    }
  }
} hello_encoder;

// Toggle electro-magnet every 2 seconds

#include "rodos.h"

HAL_GPIO magnet(GPIO_064); // PE0

bool output = false;

class HelloMagnet : public StaticThread<>
{
  void init()
  {
    magnet.init(true, 1, 0);
  }

  void run()
  {
    init();

    TIME_LOOP(NOW(), 2000 * MILLISECONDS)
    {
      magnet.setPins(output);
      output = !output;
    }
  }
}hello_magnet;

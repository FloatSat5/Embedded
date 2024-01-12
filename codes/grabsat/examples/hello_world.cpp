#include "rodos.h"

class HelloWorld : public StaticThread<>
{
  void run()
  {
    PRINTF("Hello World!\n");
  }
} hello_world;

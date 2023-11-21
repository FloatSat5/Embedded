// TB9051FTG in drive-brake (slow decay) operation

#include "rodos.h"
#include "motor.h"

Motor rw(PWM_IDX13, PWM_IDX14); // PD13 & PD14
const uint16_t duty_cycle = 30;
bool reverse_flag = true;

class HelloMotor : public StaticThread<>
{
public:
  void init()
  {
    const uint16_t frequency = 2000;
    const uint16_t increments = 1000;

    rw.set_frequency(frequency);
    rw.set_increments(increments);
  }

  void run()
  {
    init();

    TIME_LOOP(NOW(), 5000 * MILLISECONDS)
    {
      if (reverse_flag)
      {
        rw.set_duty_cycle(-1 * duty_cycle);
      }
      else
      {
        rw.set_duty_cycle(duty_cycle);
      }
      reverse_flag = !reverse_flag;
    }
  }
} hello_motor;

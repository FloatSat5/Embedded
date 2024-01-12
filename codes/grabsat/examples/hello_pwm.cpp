// PWM test by changing the LED brightness

#include "rodos.h"

// LEDs on discovery board
HAL_PWM pwm_green(PWM_IDX12);
HAL_PWM pwm_orange(PWM_IDX13);
HAL_PWM pwm_red(PWM_IDX14);
HAL_PWM pwm_blue(PWM_IDX15);

class LedPwm : public StaticThread<>
{
public:
  uint8_t duty_cycle = 0;

  void init()
  {
    uint16_t frequency = 1000;
    uint16_t increments = 100;

    pwm_green.init(frequency, increments);
    pwm_orange.init(frequency, increments);
    pwm_red.init(frequency, increments);
    pwm_blue.init(frequency, increments);
  }

  void run()
  {
    init();

    TIME_LOOP(100 * MILLISECONDS, 50 * MILLISECONDS)
    {
      pwm_green.write(duty_cycle);
      pwm_orange.write(duty_cycle);
      pwm_red.write(duty_cycle);
      pwm_blue.write(duty_cycle);

      duty_cycle++;
      if (duty_cycle > 100)
      {
        duty_cycle = 0;
      }
    }
  }
} led_pwm;

#include "arm.h"
#include "ft90mr.h"
#include "satellite_config.h"

// Servo motors
ft90mr magnet_arm(SERVO_ARM_PWM_IDX);
ft90mr balance_arm(SERVO_BALANCE_PWM_IDX);

HAL_GPIO pin_green_led(GPIO_060);
HAL_GPIO pin_orange_led(GPIO_061);

void ArmThread::init()
{
  pin_green_led.init(true, 1, 0);
  pin_orange_led.init(true, 1, 0);
}

void ArmThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      pin_green_led.setPins(0);
      pin_orange_led.setPins(0);

      magnet_arm.run(0);
      balance_arm.run(0);

      suspendCallerUntil(END_OF_TIME);
    }

    if (dir_flag) // extend
    {
      pin_orange_led.setPins(1);
      magnet_arm.run(SERVO_ARM_SPEED);
      balance_arm.run(SERVO_ARM_SPEED);
    }
    else // retract
    {
      pin_green_led.setPins(1);
      magnet_arm.run(-SERVO_ARM_SPEED);
      balance_arm.run(-SERVO_ARM_SPEED);
    }

    stop_flag = true; // Stop after one execution
    suspendCallerUntil(NOW() + period * MILLISECONDS);
  }
}

ArmThread arm_thread;

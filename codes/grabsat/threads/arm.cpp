#include "arm.h"
#include "ft90mr.h"
#include "satellite_config.h"

// Servo motors
ft90mr magnet_arm(SERVO_ARM_PWM_IDX);
ft90mr balance_arm(SERVO_BALANCE_PWM_IDX);

HAL_GPIO magnet_switch(SERVO_ARM_GPIO);
HAL_GPIO balance_switch(SERVO_BALANCE_GPIO);

void ArmThread::init()
{
  magnet_switch.init(false, 1, 0);
  balance_switch.init(false, 1, 0);
}

void ArmThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      magnet_arm.run(0);
      balance_arm.run(0);
      suspendCallerUntil(END_OF_TIME);
    }

    int dir = 0;
    if (dir_flag) // extend
    {
      magnet_arm.run(SERVO_ARM_SPEED);
      balance_arm.run(SERVO_ARM_SPEED);
      dir = -1;
    }
    else // retract
    {
      dir = 1;
    }

  if(!magnet_switch.readPins())
  {
    magnet_arm.run(0);
  }
  else // Magnet switch not pressed
  {
    magnet_arm.run(dir * SERVO_ARM_SPEED);
  }

  if(!balance_switch.readPins())
  {
    balance_arm.run(0);
  }
  else // Balance switch not pressed
  {
    balance_arm.run(dir * SERVO_ARM_SPEED);
  }

    stop_flag = true; // Stop after one execution
    suspendCallerUntil(NOW() + period * MILLISECONDS);
  }
}

ArmThread arm_thread;

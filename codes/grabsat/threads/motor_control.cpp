#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "telecommand.h"
#include "motor_control.h"
#include "satellite_config.h"

pid rw_pid;
Motor rw(RW_PWM1_IDX, RW_PWM2_IDX);

void MotorControlThread::init()
{
  // PID configuration
  rw_pid.set_gains(telecommands[gkpmw].value, telecommands[gkimw].value, 0.0);
  rw_pid.set_control_limits(0, PID_MOTOR_UMAX);

  // Motor driver configuration
  rw.set_frequency(RW_PWM_FREQUENCY);
  rw.set_frequency(RW_PWM_INCREMENTS);

  // Motor encoder configuration
  encoder::init();
}

void MotorControlThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      suspendCallerUntil(END_OF_TIME);
    }

    // Compute error
    const float dt = PERIOD_MOTOR_CONTROL / 1000.0;
    const float omega = encoder::get_omega(ENCODER_CPR, dt);
    const float set_point = telecommands[mosav].value;
    const float error = set_point - omega;

    // Compute control command and actuate
    rw_pid.set_gains(telecommands[gkpmw].value, telecommands[gkimw].value, 0.0);
    const float pwm = rw_pid.update(error, dt);
    rw.set_duty_cycle(pwm);

    PRINTF("%f %f\n", set_point, omega);
    suspendCallerUntil(NOW() + PERIOD_MOTOR_CONTROL * MILLISECONDS);
  }
}

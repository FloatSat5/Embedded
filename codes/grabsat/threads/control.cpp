#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "control.h"
#include "satellite.h"
#include "telecommand.h"
#include "satellite_config.h"

pid m_pid; // Motor rate control
pid w_pid; // Satellite rate control
pid y_pid; // Satellite yaw control
Motor rw(RW_PWM1_IDX, RW_PWM2_IDX);

void ControlThread::init()
{
  // PID configuration
  m_pid.set_gains(telecommands[gkpmw].value, telecommands[gkimw].value, 0.0);
  m_pid.set_control_limits(0, PID_MOTOR_UMAX);

  // Motor driver configuration
  rw.set_frequency(RW_PWM_FREQUENCY);
  rw.set_frequency(RW_PWM_INCREMENTS);

  // Motor encoder configuration
  encoder::init();
}

// Performs one of three control actions
void ControlThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      // Forget PID past values
      m_pid.reset_memory();
      w_pid.reset_memory();
      y_pid.reset_memory();

      suspendCallerUntil(END_OF_TIME);
    }

    const float dt = PERIOD_MOTOR_CONTROL / 1000.0;
    float m_sp = 0.0; // Motor omega set-point

    // Select motor omega set-point
    if(current_mode == satellite_mode::motor)
    {
      m_sp = telecommands[mosav].value;
    }
    else if(current_mode == satellite_mode::omega)
    {
      const float w = 0.0;
      const float w_sp = telecommands[sangv].value;
      const float w_err = w_sp - w;

      m_sp = w_pid.update(w_err, dt);
    }
    else if(current_mode == satellite_mode::yaw)
    {
      const float y = 0.0;
      const float y_sp = telecommands[sangv].value;
      const float y_err = y_sp - y;

      m_sp = y_pid.update(y_err, dt);
    }
    else // Just in case
    {
      suspendCallerUntil(END_OF_TIME);
    }

    // Compute motor omega and error
    const float m_w = encoder::get_omega(ENCODER_CPR, dt);
    const float m_err = m_sp - m_w;

    // Update gains (if changed using telecommand)
    m_pid.set_gains(telecommands[gkpmw].value, telecommands[gkimw].value, 0.0);

    // Perform PID algorithm and actuate
    const float pwm = m_pid.update(m_err, dt);
    rw.set_duty_cycle(pwm);

    PRINTF("%f %f\n", m_sp, m_w);
    suspendCallerUntil(NOW() + PERIOD_MOTOR_CONTROL * MILLISECONDS);
  }
}

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

// Satellite angular rate control outer loop
float omega_control(const float dt)
{
  const float w = 0.0;
  const float w_sp = telecommands[sangv].value;
  const float w_err = w_sp - w;

  return w_pid.update(w_err, dt);
}

// Satellite yaw control outer loop
float position_control(const float dt)
{
  const float y = 0.0;
  const float y_sp = telecommands[sangp].value;
  const float y_err = y_sp - y;

  return y_pid.update(y_err, dt);
}

// Motor omega control inner loop
float motor_control(const float m_sp, const float dt)
{
  const float m_w = encoder::get_omega_lpf(ENCODER_CPR, dt);
  const float m_err = m_sp - m_w;
  PRINTF("%f %f\n", m_sp, m_w);

  // Update gains (if changed using telecommand)
  m_pid.set_gains(telecommands[gkpmw].value, telecommands[gkimw].value, 0.0);

  return m_pid.update(m_err, dt);
}

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
  encoder::set_lpf_fc(ENCODER_LPF_FC);
}

// Performs one of three control actions
void ControlThread::run()
{
  while (1)
  {
    if (stop_flag)
    {
      rw.brake();
      m_pid.reset_memory();
      w_pid.reset_memory();
      y_pid.reset_memory();
      encoder::reset_count();

      suspendCallerUntil(END_OF_TIME);
    }

    const float dt = PERIOD_MOTOR_CONTROL / 1000.0;
    float m_sp = 0.0; // Motor omega set-point

    // Select motor omega set-point
    if (current_mode == satellite_mode::motor)
    {
      m_sp = telecommands[mosav].value;
    }
    else if (current_mode == satellite_mode::omega)
    {
      m_sp = omega_control(dt);
    }
    else if (current_mode == satellite_mode::yaw)
    {
      m_sp = position_control(dt);
    }
    else // Just in case
    {
      suspendCallerUntil(END_OF_TIME);
    }

    // Perform inner control loop and actuate motor
    float pwm = motor_control(m_sp, dt);
    rw.set_duty_cycle(pwm);

    suspendCallerUntil(NOW() + PERIOD_MOTOR_CONTROL * MILLISECONDS);
  }
}

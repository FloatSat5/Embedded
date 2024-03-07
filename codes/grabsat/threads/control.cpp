#include "pid.h"
#include "motor.h"
#include "topics.h"
#include "encoder.h"
#include "lanczos.h"
#include "lsm9ds1.h"
#include "mahony.h"
#include "control.h"
#include "satellite.h"
#include "telecommand.h"
#include "multimeter.h"
#include "satellite_config.h"

#define D2R 0.01745329251f
#define R2D 57.2957795131f

telemetry_struct telemetry_tx;

pid m_pid; // Motor rate control
pid w_pid; // Satellite rate control
pid y_pid; // Satellite yaw control
Motor rw(RW_PWM1_IDX, RW_PWM2_IDX);

const float kp = 30;
const float ki = 0;
mahony filter(kp, ki);

// Satellite angular rate control outer loop
float omega_control(const float w, const float dt)
{
  const float w_sp = telecommands[sangv].value;
  const float w_err = w_sp - w;

  // Update gains (if changed using telecommand)
  w_pid.set_kp(telecommands[gkpsw].value);
  w_pid.set_ki(telecommands[gkisw].value);

  float sp = -w_pid.update(w_err, dt);

  return sp;
}

float get_yaw(const float dt)
{
  float a[3], g[3], m[3];
  lsm9ds1_read_accel(a);
  lsm9ds1_read_gyro(g);
  lsm9ds1_read_mag(m);

  g[0] = g[0] * D2R * 0.1;
  g[1] = g[1] * D2R * 0.1;
  g[2] = g[2] * D2R * 0.1;

  g[0] = -g[0];
  a[0] = -a[0];

  float ypr[3];
  filter.update(a, g, m, dt);
  filter.get_ypr(ypr);

  lnz::Euler ea321({0, ypr[1], ypr[2]});
  lnz::Vector<3> mv({m[0], m[1], m[2]});
  mv = trans(ea321.get_dcm()) * mv;

  float psi = atan2(-mv(1), mv(0));
  if (psi < 0)
  {
    psi += 2 * M_PI;
  }

  return psi;
}

// Satellite yaw control outer loop
float position_control(const float y, const float dt)
{
  const float y_sp = telecommands[sangp].value;
  float y_err = y_sp - y;

  // Is it the shortest path?
  if (y_err >= 180)
  {
    y_err -= 2 * 180;
  }
  else if (y_err < -180)
  {
    y_err += 2 * 180;
  }
  y_err = -y_err;

  // Update gains (if changed using telecommand)
  y_pid.set_kp(telecommands[gkpsa].value);
  y_pid.set_ki(telecommands[gkisa].value);
  y_pid.set_kd(telecommands[gkdsa].value);

  return y_pid.update(y_err, dt);
}

// Motor omega control inner loop
float motor_control(const float m_sp, const float m_w, const float dt)
{
  const float m_err = m_sp - m_w;

  // Update gains (if changed using telecommand)
  m_pid.set_kp(telecommands[gkpmw].value);
  m_pid.set_ki(telecommands[gkimw].value);

  return m_pid.update(m_err, dt);
}

void get_ypr(const float dt, float ypr[3])
{
  float a[3], g[3], m[3];
  lsm9ds1_read_accel(a);
  lsm9ds1_read_gyro(g);
  lsm9ds1_read_mag(m);

  g[0] = g[0] * D2R * 0.1;
  g[1] = g[1] * D2R * 0.1;
  g[2] = g[2] * D2R * 0.1;

  g[0] = -g[0];
  a[0] = -a[0];

  // float ypr[3];
  filter.update(a, g, m, dt);
  filter.get_ypr(ypr);

  lnz::Euler ea321({0, ypr[1], ypr[2]});
  lnz::Vector<3> mv({m[0], m[1], m[2]});
  mv = trans(ea321.get_dcm()) * mv;

  float psi = atan2(-mv(1), mv(0));
  if (psi < 0)
  {
    psi += 2 * M_PI;
  }

  ypr[0] = psi;
}

void ControlThread::init()
{
  // PID configuration
  m_pid.set_control_limits(PID_MOTOR_UMIN, PID_MOTOR_UMAX);
  w_pid.set_control_limits(PID_OMEGA_UMIN, PID_OMEGA_UMAX);
  y_pid.set_control_limits(PID_OMEGA_UMIN, PID_OMEGA_UMAX);

  // Motor driver configuration
  rw.set_frequency(RW_PWM_FREQUENCY);
  rw.set_frequency(RW_PWM_INCREMENTS);

  // Motor encoder configuration
  encoder::init();
  encoder::set_lpf_fc(ENCODER_LPF_FC);

  // Multimeter
  multimeter::init();

  // IMU init
  lsm9ds1_init();

  filter.normalize_imu();
}

// Performs one of three control actions
void ControlThread::run()
{
  const float dt = PERIOD_MOTOR_CONTROL / 1000.0;

  while (1)
  {
    // Angular velocity
    float g[3] = {0.0};
    lsm9ds1_read_gyro(g);
    g[2] -= YAW_GYRO_BIAS;

    // Orientation
    float ypr[3] = {0.0};
    get_ypr(dt, ypr);

    // Motor angular velocity
    const float m_w = encoder::get_omega_lpf(ENCODER_CPR, dt);

    // Send the data to telemetry
    telemetry_tx.w = m_w;
    telemetry_tx.g[0] = g[0];
    telemetry_tx.g[1] = g[1];
    telemetry_tx.g[2] = g[2];
    telemetry_tx.ypr[0] = ypr[0];
    telemetry_tx.ypr[1] = ypr[1];
    telemetry_tx.ypr[2] = ypr[2];
    telemetry_topic.publish(telemetry_tx);

    if (stop_flag)
    {
      rw.brake();
      m_pid.reset_memory();
      w_pid.reset_memory();
      y_pid.reset_memory();
      encoder::reset_count();
      stop_flag = false;
    }
    else
    {
      bool actuate_motor = true;
      float m_sp = 0.0; // Motor omega set-point

      // Select motor omega set-point
      if (current_mode == satellite_mode::motor)
      {
        m_sp = telecommands[mosav].value;
      }
      else if (current_mode == satellite_mode::omega)
      {
        m_sp = omega_control(g[2], dt);
      }
      else if (current_mode == satellite_mode::yaw)
      {
        m_sp = position_control(ypr[0] * R2D, dt);
      }
      else if (current_mode == satellite_mode::debris)
      {
        float temp_yaw_Command = ypr[0] * R2D + telecommands[fideb].value;

        while(temp_yaw_Command > 360)
        {
          temp_yaw_Command -= 360;
        }

        while(temp_yaw_Command < 0)
        {
          temp_yaw_Command += 360;
        }

        telecommands[sangp].value = temp_yaw_Command;
        current_mode = satellite_mode::yaw;
      }
      else
      {
        actuate_motor = false;
      }

      // Perform inner control loop and actuate motor
      if (actuate_motor)
      {
        float pwm = motor_control(m_sp, m_w, dt);
        rw.set_duty_cycle(pwm);
      }
    }

    suspendCallerUntil(NOW() + PERIOD_MOTOR_CONTROL * MILLISECONDS);
  }
}

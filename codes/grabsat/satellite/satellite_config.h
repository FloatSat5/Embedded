// Satellite configuration file
// Rishav (2024-01-24)

#ifndef _SATELLITE_CONFIG_H_
#define _SATELLTIE_CONFIG_H_

/*
  PIN-OUTS AND LOW-LEVEL CONFIG
*/

// Telemetry buffer length
#define BLUETOOTH_BUFFER_LEN 200

// Reaction wheel
#define RW_PWM1_IDX PWM_IDX02 // PE13
#define RW_PWM2_IDX PWM_IDX03 // PE14
#define RW_PWM_FREQUENCY 2000 // Hz
#define RW_PWM_INCREMENTS 1000

// Encoder
#define ENCODER_CPR 64 // Counts per revolution
#define ENCODER_LPF_FC 0.9 // Check tune_lpf.m

// Servo motors
#define SERVO_ARM_PWM_IDX PWM_IDX08 // PA6
#define SERVO_BALANCE_PWM_IDX PWM_IDX10 // PB0
#define SERVO_ARM_SPEED 30
#define SERVO_ARM_RUN_RIME 3 // s

// Multimeter
#define MULTIMETER_ADC_IDX ADC_IDX1 // PC4
#define MULTIMETER_ADC_CH ADC_CH_014 // Channel
#define MULTIMETER_R1 330e3 // Ohm
#define MULTIMETER_R2 1.33e6 // Ohm
#define MULTIMETER_MAX_VOL 3.0 // V
#define MULTIMETER_MAX_ADC 4096.0

// Groundstation
#define GROUNDSTATION_UART_IDX UART_IDX4
#define GROUNDSTATION_UART_BAUD 115200

/*
  THREAD CONFIGURATIONS
*/

// Thread periods, ms
#define PERIOD_GROUNDSTATION 50
#define PERIOD_MOTOR_CONTROL 25
#define PERIOD_TELEMETRY_CONTROL 100

/*
  PID control gains
*/

// Motor angular rate
#define PID_MOTOR_KP 5
#define PID_MOTOR_KI 0.2
#define PID_MOTOR_UMAX 100.0

// Satellite angular
#define PID_OMEGA_KP 1.0
#define PID_OMEGA_KI 0.5

// Satellite angle
#define PID_YAW_KP 1
#define PID_YAW_KI 0.0

#endif // satellite_config.h

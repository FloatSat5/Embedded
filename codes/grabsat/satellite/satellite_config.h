// Satellite configuration file
// Rishav (2024-01-24)

#ifndef _SATELLITE_CONFIG_H_
#define _SATELLTIE_CONFIG_H_

/*
  PIN-OUTS AND LOW-LEVEL CONFIG
*/

// me: 1, team:2
#define WHO_IS_USER 2

// Telemetry buffer length
#define BLUETOOTH_BUFFER_LEN 200

// Reaction wheel
#define RW_PWM1_IDX PWM_IDX02 // PE13
#define RW_PWM2_IDX PWM_IDX03 // PE14
#define RW_PWM_FREQUENCY 5000 // Hz
#define RW_PWM_INCREMENTS 1000

// Encoder
#define ENCODER_CPR 64 // Counts per revolution
#define ENCODER_LPF_FC 0.9 // Check tune_lpf.m

// Servo motors
#define SERVO_BALANCE_PWM_IDX PWM_IDX08 // PA6
#define SERVO_ARM_PWM_IDX PWM_IDX10 // PB0
#define SERVO_ARM_SPEED 30
#define SERVO_ARM_RUN_RIME 3 // s
#define SERVO_ARM_GPIO GPIO_046 // PC14
#define SERVO_BALANCE_GPIO GPIO_045 // PC13

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

// Raspberry Pi communication
#define RPI_COMM_UART_IDX UART_IDX6 // Tx: PC6, Rx:PC7
#define RPI_COMM_UART_BAUD 115200
#define RPI_COMM_INVALID 555

/*
  THREAD CONFIGURATIONS
*/

// Thread periods, ms
#define PERIOD_GROUNDSTATION 50
#define PERIOD_MOTOR_CONTROL 25
#define PERIOD_TELEMETRY_CONTROL 100
#define PERIOD_RPI_COMM 50

/*
  PID control gains

  Observations:
    1. Relatively larger I compared to P creates ramping effect with less overshoot.
*/

// Motor angular rate
#define PID_MOTOR_KP 1.5
#define PID_MOTOR_KI 1.5
#define PID_MOTOR_UMAX 100.0 // PWM limits
#define PID_MOTOR_UMIN -100.0

// Satellite angular
#define PID_OMEGA_KP 0.1
#define PID_OMEGA_KI 0.4
#define PID_OMEGA_UMAX 90.0 // Motor omega limits
#define PID_OMEGA_UMIN -90.0

// Satellite angle
#define PID_YAW_KP 1.0
#define PID_YAW_KI 0.2
#define PID_YAW_KD 0.0

#endif // satellite_config.h

# Embedded

### codes/grabsat
1. [examples](codes/grabsat/examples) - Test software for different components
2. [rodos](codes/grabsat/rodos) - RODOS source code
3. [libs](codes/grabsat/libs) - Custom drivers and algorithms used in GRABSAT-I
4. [satellite](codes/visualizer) - Libraries to handle GRABSAT-I mission
5. [visualizer](codes/visualizer) - Simple attitude visualizer

### codes/grabsat/libs
1. [lsm9ds1](/codes/grabsat/libs/lsm9ds1) - LSM9DS1 IMU + magnetometer interface
2. [pid](/codes/grabsat/libs/pid) - Discrete PID controller
3. [motor](/codes/grabsat/libs/motor) - TB9051FTG interface for drive-brake (slow decay) operation.
4. [madgwick](/codes/grabsat/libs/madgwick/) - Implementation of Madgwick's filter
5. [rc_lpf](/codes/grabsat/libs/rc_lpf/) - RC low-pass filter for smoothing angular velocity
6. [encoder](/codes/grabsat/libs/encoder/) - Encoder interface using STM32F4's encoder mode
7. [ft90mr](/codes/grabsat/libs/ft90mr/) - Driver for FT90MR $360^{\circ}$ servo motor

### codes/grabsat/satellite
1. [telecommand](/codes/grabsat/satellite/telecommand.h) - Handles the telecommands from groundstation to GRABSAT-I
2. [satellite](/codes/grabsat/satellite/satellite.h) - Handles the missions and modes of operation of GRABSAT-I
3. [telemetry](/codes/grabsat/satellite/telemetry.h) - Handles telemetry to ground station
4. [multimeter](/codes/grabsat/satellite/multimeter.h) - Measures battery current and voltage
5. [satellite_config](/codes/grabsat/satellite/satellite_config.h) - Configuration file for satellite peripherals and control gains

### codes/grabsat/threads
1. [groundstation](/codes/grabsat/threads/groundstation.h) - Communication with groundstation
2. [motor_control](/codes/grabsat/threads/motor_control.h) - Motor angular velocity control
3. [switch_board](/codes/grabsat/threads/switch_board.h) - Interface to enable/disable threads. **Not a thread!**

### Todos
1. Telemetry handler
2. Design and document control law for satellite $\psi$ and $\omega$
3. Test attitude estimation

### Dependencies
1. <code>make</code> - To compile and flash code to STM32
2. <code>arm-none-eabi-gcc</code> - Compiler for STM32
2. <code>openocd</code> - To flash the compiled .hex/.bin files to STM32

### Usage
1. Examples (e.g. hello_blinky.cpp)
  - <code>make clean</code> followed by <code>make main=hello_blinky.cpp</code> to compile software
  - <code>make flash</code> to flash software to STM32

2. GRABSAT-1 software
  - <code>make</code> to compile software
  - <code>make flash</code> to flash software to STM32

Note: The software is developed and tested on Linux.

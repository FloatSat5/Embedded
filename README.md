# Embedded

### codes/grabsat
1. [examples](codes/grabsat/examples) - Test software for different components
2. [rodos](codes/grabsat/rodos) - RODOS source code
3. [libs](codes/grabsat/libs) - Custom drivers and algorithms used in GRABSAT-I
4. [satellite](codes/visualizer) - Libraries to handle GRABSAT-I mission
5. [visualizer](codes/visualizer) - Simple attitude visualizer

### codes/grabsat/libs
1. [lsm9ds1](/codes/grabsat/libs/lsm9ds1) - LSM9DS1 IMU + magnetometer interface
2. [pid](/codes/grabsat/libs/pid) - PID controller
3. [motor](/codes/grabsat/libs/motor) - TB9051FTG interface for drive-brake (slow decay) operation.
4. [madgwick](/codes/grabsat/libs/madgwick/) - Implementation of Madgwick's filter
5. [rc_lpf](/codes/grabsat/libs/rc_lpf/) - RC low-pass filter for smoothing angular velocity
6. [encoder](/codes/grabsat/libs/encoder/) - Encoder interface using STM32F4's encoder mode


### codes/grabsat/satellite
1. [telecommand](/codes/grabsat/satellite/telecommand.h) - Handles the telecommands from groundstation to GRABSAT-I
1. [satellite](README.md) - Handles the missions and modes of operation of GRABSAT-I
1. [telemetry](README.md) - Handles telemetry to ground station
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

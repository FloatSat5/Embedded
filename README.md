# Embedded

### Folder structure
1. [codes/examples](/codes/examples) - Test software for each component
2. [codes/rodos](/codes/rodos/) - RODOS source code
3. [codes/libs](/codes/libs/) - Custom libraries for GRABSAT-I
3. [codes/visualizer](/codes/visualizer/) - Simple attitude visualizer

### Libraries
1. [lsm9ds1](/codes/libs/lsm9ds1) - LSM9DS1 IMU + magnetometer interface
2. [pid](/codes/libs/pid) - PID controller
3. [motor](/codes/libs/motor) - TB9051FTG interface for drive-brake (slow decay) operation.
3. [madgwick](/codes/libs/madgwick/) - Implementation of Madgwick's filter

### Todos
1. Trajectory tracking of motor $\omega$.
2. Satellite angular position/velocity control simulation.
3. Reliable yaw estimation using EKF.
4. Parser and encoder for telecommand/telemetry data format.

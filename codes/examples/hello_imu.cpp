// Basic I2C communication test with LSM9DS1 IMU

#include "rodos.h"

HAL_I2C IMU(I2C_IDX2); // Init I2C

#define IMU_ADDRESS_GYRO 0x6B      // Accel/gyro address
#define IMU_ADDRESS_ACCEL_MAG 0x1E // Magnetometer address
uint8_t IMU_WHO_AM_I[1] = {0x0F};  // ID register

class HelloIMU : public StaticThread<>
{
  void init_imu();
  void talkToIMU();

  void run()
  {
    init_imu();
    talkToIMU();
  }
} hello_imu;

void HelloIMU::init_imu()
{
  pin_dc_dc.init(true, 1, 1);
  pin_en_imu.init(true, 1, 1);
  pin_cs_ag.init(true, 1, 1);
  pin_cs_m.init(true, 1, 1);

  IMU.init(400000);
}

void HelloIMU::talkToIMU()
{
  uint8_t rx_gyro_id[1] = {0};
  uint8_t rx_accel_mag_id[1] = {0};

  int status_gyro = IMU.writeRead(IMU_ADDRESS_GYRO, IMU_WHO_AM_I, 1, rx_gyro_id, 1);
  int status_mag = IMU.writeRead(IMU_ADDRESS_ACCEL_MAG, IMU_WHO_AM_I, 1, rx_accel_mag_id, 1);

  PRINTF("Gyro_id = 0x%x with I2C_status = %d (Expected = 0x68, 1)\r\n", rx_gyro_id[0], status_gyro);          // 11010100 -> 0xD4
  PRINTF("Accel_mag_id = 0x%x with I2C_status = %d (Expected = 0x3D, 1)\r\n", rx_accel_mag_id[0], status_mag); // 01001001 -> 0x49

  if ((rx_gyro_id[0] == 0x68) && (rx_accel_mag_id[0] == 0x3D))
  {
    PRINTF("Successful communication with IMU !! \r\n\n");
  }
  else
  {
    PRINTF("IMU communication error !! \r\n\n");
  }
}

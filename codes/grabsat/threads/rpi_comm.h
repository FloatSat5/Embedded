// Communication thread with Raspberry Pi
// 2023-02-07

#ifndef _RPI_COMM_H_
#define _RPI_COMM_H_

#include "rodos.h"
#include "satellite_config.h"

struct rpi_data_t
{
  float del = RPI_COMM_INVALID_ANGLE; // Angular distance to debris, degrees
  float dis = 0.0; // Camera to debris distance, cm
  int psi = 0; // Star tracker angle, degrees
};

extern rpi_data_t rpi_data;

class RpiCommThread : public StaticThread<>
{
private:
  bool parse(const uint8_t *msg, int n);
  bool populate_rpi_data(const char *idx, const float value);
  void print(bool parse_status);

public:
  bool stop_flag = true;

  void run();
  void init();
};

extern RpiCommThread rpi_command_thread;

#endif // rpi_comm.h

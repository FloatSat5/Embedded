// Communication thread with Raspberry Pi
// 2023-02-07

#ifndef _RPI_COMM_H_
#define _RPI_COMM_H_

#include "rodos.h"
#include "satellite_config.h"

struct rpi_data_t
{
  float del = RPI_COMM_INVALID; // Angular distance to debris, degrees
  float dis = RPI_COMM_INVALID; // Camera to debris distance, cm
  int psi = RPI_COMM_INVALID; // Star tracker angle, degrees
  int x = 0; // Checksum
};

enum rpi_mode
{
  STAR_TRACKER = 2,
  DEBRIS = 1,
  IDLE = 3
};

extern rpi_data_t rpi_data;

class RpiCommThread : public StaticThread<>
{
private:
  void print(bool parse_status);
  bool parse(const uint8_t *msg, int n);
  bool populate_rpi_data(const char *idx, const float value);

public:
  bool stop_flag = false;
  void transmit(rpi_mode rm);

  void run();
  void init();
};

extern RpiCommThread rpi_command_thread;

#endif // rpi_comm.h

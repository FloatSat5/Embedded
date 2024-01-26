// STM32 encoder mode for pins PE9 and PE11 using TIM1
// Rishav (2024-01-19)

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <inttypes.h>

#define RPM2W 0.10471975512 // RPM to rad/s
#define W2RPM 9.54929658551 // rad/s to RPM

namespace encoder
{
  void init(void);
  void reset_count(void);
  int32_t get_count(void);
  float get_omega(const uint16_t cpr, const float dt);

  // Angular velocity with LPF
  void set_lpf_fc(const float fc);
  float get_omega_lpf(const uint16_t cpr, const float dt);
}

#endif // encoder.h

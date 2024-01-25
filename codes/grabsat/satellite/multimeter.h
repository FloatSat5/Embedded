// Voltage and current measurements
// Rishav (2024-01-25)

#ifndef _MULTIMETER_H_
#define _MULTIMETER_H_

namespace multimeter
{
  void init(void);
  float get_voltage(void);
  float get_current(void);
}

#endif // multimeter.h

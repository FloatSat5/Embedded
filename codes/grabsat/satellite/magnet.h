// Electromagnet interface
// 2024-01-26

#ifndef _MAGNET_H_
#define _MAGNET_H_

namespace magnet
{
  extern bool state;

  void init(void);
  void actuate(const bool on_off);
}

#endif // magnet.h

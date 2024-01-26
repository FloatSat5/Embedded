#include "utils.h"

// Gets away with possible problem due to float precision
bool utils::float_to_bool(const float in)
{
  const float tolerance = 0.1;

  if(in < tolerance)
  {
    return false;
  }

  return true;
}
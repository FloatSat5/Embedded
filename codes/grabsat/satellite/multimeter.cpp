#include "rodos.h"
#include "multimeter.h"
#include "satellite_config.h"

#include <inttypes.h>

HAL_ADC multimeter_adc(MULTIMETER_ADC_IDX);

void multimeter::init(void)
{
  multimeter_adc.init(MULTIMETER_ADC_CH);
}

float multimeter::get_current(void)
{
  return 0.0;
}

float multimeter::get_voltage(void)
{
  uint16_t adc = multimeter_adc.read(MULTIMETER_ADC_CH);
  float r1 = MULTIMETER_R1;
  float r2 = MULTIMETER_R2;
  float max_vol = MULTIMETER_MAX_VOL;
  float min_step = max_vol / MULTIMETER_MAX_ADC;
  float v_out = min_step * adc;
  float v_in = (r1 + r2) * v_out / r1;

  return v_in;
}

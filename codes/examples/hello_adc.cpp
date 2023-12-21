// ADC example for voltage sampling

#include "rodos.h"
#include "stm32f4xx.h"
#include <math.h>

// PC4
HAL_ADC adc(ADC_IDX1);

double get_voltage(const uint16_t adc)
{
  double r1 = 330e3;
  double r2 = 1.33e6;
  double max_vol = 3.0;
  double min_step = max_vol / 4096.0;
  double v_out = min_step * adc;
  double v_in = (r1 + r2) *  v_out / r1; 

  // PRINTF("%d %lf %lf\n", adc, v_in, v_out);
  PRINTF("%lf %lf\n", v_in, v_out);

  return v_in;
} 

class HelloADC : public StaticThread<>
{
  void init()
  {
    adc.init(ADC_CH_014);
  }

  void run()
  {
    init();
    TIME_LOOP(NOW(), 15 * MILLISECONDS)
    {
        uint16_t adc_val = adc.read(ADC_CH_014);
        get_voltage(adc_val);
    }
  }
} hello_adc;

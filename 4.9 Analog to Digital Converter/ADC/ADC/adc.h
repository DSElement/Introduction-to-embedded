#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void ADC_Init(void);
uint32_t ADC_Read();
uint32_t adc_to_mv(uint32_t adc_value, uint8_t resolution);


#endif

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint32_t adc_to_mv(uint32_t adc_value, uint8_t resolution);


#endif

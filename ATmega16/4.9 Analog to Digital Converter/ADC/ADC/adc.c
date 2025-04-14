#include "ADC.h"

#define VOLTAGE_HIGH 5000

void ADC_Init(void) {
	DDRA &= ~(1 << 0);		 //set as input
    ADMUX |= (1 << REFS0);    //select channel
    ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);  //set prescalers and aden
}

uint32_t ADC_Read() {
    uint32_t result = 0;
	ADCSRA |= (1 << ADSC); //start conversion
	while (ADCSRA & (1 << ADSC)); //wait for conversion to be comlpeted
    result = ADCL;
    result |= (ADCH << 8);
    return result;
}

uint32_t adc_to_mv(uint32_t adc_value, uint8_t resolution) {
    return ((uint32_t)adc_value * 5000) >> resolution; // Convert to mV (assuming Vref = 5V)
}

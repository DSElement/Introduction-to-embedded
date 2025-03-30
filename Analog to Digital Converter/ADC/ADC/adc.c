#include "ADC.h"
//#include "uart.h"

#define VOLTAGE_HIGH 5000

void ADC_Init(void) {
	
    // Set AVCC as reference, right adjust result, start with channel 0
    //ADMUX = (1 << REFS0);
    
    // Enable ADC, set prescaler to 8 (ADPS2 = 0, ADPS1 = 1, ADPS0 = 1)
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	//uart_init();
}

uint16_t ADC_Read(uint8_t channel) {
    // Select ADC channel (only 0-7 valid)
    //ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
	ADMUX |= channel;
	ADMUX |= (1 << REFS0);
	
    
    // Start conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    //while (ADCSRA & (1 << ADSC));
	while((ADCSRA >> ADSC ) & 1);
    
    // Read ADC result (ADCL must be read first, then ADCH)
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    
    return result;
}

uint32_t adc_to_mv(uint32_t adc_value, uint8_t resolution) {
    //return (adc_value * 5000) >> resolution; // Convert to mV (assuming Vref = 5V)
	return (adc_value * 5000) / ((1 << resolution)-1);
}

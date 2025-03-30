#include "ADC.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>

int main() {
    ADC_Init();
    uart_init();
	
	DDRA = 0;
	
	char buffer[20];
    
    while (1) {
		/*uint16_t adc_value = ADC_Read(0); // Read from channel 0
		uart_transmit((adc_value >> 8) & 0xFF); // Send high byte
		uart_transmit(adc_value & 0xFF);       // Send low byte
		_delay_ms(1000);*/
		
        uint32_t adc_value = ADC_Read(0); // Read from channel 0
		//uart_transmit((adc_value >> 8) & 0xFF); // Send high byte
		//uart_transmit(adc_value & 0xFF);       // Send low byte
		uint32_t voltage = adc_to_mv(adc_value, 10);
		sprintf(buffer, "VOLTAGE = %lu mV\r\n", voltage);
		uart_transmit_string(buffer);
        _delay_ms(1000); // Wait 1 second
    }
    
    return 0;
}

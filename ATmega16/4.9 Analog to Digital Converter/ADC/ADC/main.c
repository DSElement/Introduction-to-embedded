#include "ADC.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>

int main() {
    uart_init();
	ADC_Init();
	char buffer[20];
    while (1) {
        uint32_t adc_value = ADC_Read();
		uint32_t voltage = adc_to_mv(adc_value, 10);
		sprintf(buffer, "VOLTAGE = %u mV\r\n", voltage);
		uart_transmit_string(buffer);
		_delay_ms(500);
    }
    
    return 0;
}

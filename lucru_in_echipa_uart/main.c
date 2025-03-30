/*
 * uart.c
 *
 * Created: 3/21/2025 12:14:47 PM
 * Author : Dan G
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "uart.h"


int main(void)
{
	uart_init();
	/*uint16_t counter = 0;
	char tab[100];
    while (1){
		//uart_transmit('A'); // Send character 'A'
		sprintf(tab, "Iteration number: %u  ",counter++);
		uart_transmit_string(tab);
		_delay_ms(1000); // Wait 1 second
	}*/
	/*
	while (1){
		uint8_t receivedByte = uart_receive(); // Wait for a byte
		uart_transmit(receivedByte);
	}*/
	
	while (1) {
		if (uart_received_flag) { // Check if a new byte was received
			uart_transmit(uart_received_byte); // Echo received byte
			uart_received_flag = 0;
		}
	}
	
}


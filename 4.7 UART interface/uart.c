#include "uart.h"

volatile uint8_t uart_received_flag = 0;
volatile uint8_t uart_received_byte = 0;

void uart_init(void) {
	DDRD |= (1 << 1);  //PD1 as output
	DDRD &= ~(1 << 0);  //PD0 as input
	UCSRC &= ~(1 << URSEL); //permite accesu in UBRRH
	UBRRH = (uint8_t)(MYUBRR>>8);
	UBRRL = (uint8_t)MYUBRR;
	//UCSRA &= ~(1 << U2X) & ~(1 << MPCM); //Set flags on 0
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1 << RXCIE); // Activare RX și TX si rx interrupt
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); // 8-bit, 1 stop, fără paritate
	//UCSRC &= ~(1 << UCSZ2);
	sei();
}

void uart_transmit(char data) {
	while (!(UCSRA & (1<<UDRE))); // Așteaptă până buffer-ul e gol
	UDR = data;
}

void uart_transmit_string(const char *str) {
	while (*str) {
		uart_transmit(*str++);
	}
}

char uart_receive(void) {
	while (!(UCSRA & (1<<RXC))); // Așteaptă până e primit un caracter
	return UDR;
}

ISR(USART_RXC_vect) {
	uart_received_byte = UDR; // Read received byte
	uart_received_flag = 1; // Set flag to indicate new data
}

/*void uart_enable_rx_interrupt(void) {
	UCSRB |= (1<<RXCIE); // Activează întreruperea la recepție
	sei(); // Activează întreruperile globale
}*/

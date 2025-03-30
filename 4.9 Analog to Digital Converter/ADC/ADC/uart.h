#ifndef UART_H
#define UART_H

#define F_CPU 14745600UL
#define BAUD 115200
#define MYUBRR (F_CPU/BAUD/16-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t uart_received_flag;
volatile uint8_t uart_received_byte;
//ISR(USART_RXC_vect);

void uart_init(void);
void uart_transmit(char data);
void uart_transmit_string(const char *str);
char uart_receive(void);
//void uart_enable_rx_interrupt(void);

#endif

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define F_CPU 14745600UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void uart_init(uint16_t ubrr);
void uart_transmit(char data);
void uart_transmit_string(const char *str);
char uart_receive_blocking(void);
char uart_receive_nonblocking(void);
void uart_enable_rx_interrupt(void);

volatile char received_char;
volatile uint8_t data_received;

#endif

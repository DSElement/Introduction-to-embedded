#include "uart.h"

void uart_init(void) {
    UBRRH = (uint8_t)(MYUBRR>>8);
    UBRRL = (uint8_t)MYUBRR;
    UCSRB = (1<<RXEN) | (1<<TXEN); // Activare RX și TX
    UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); // 8-bit, 1 stop, fără paritate
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

void uart_enable_rx_interrupt(void) {
    UCSRB |= (1<<RXCIE); // Activează întreruperea la recepție
    sei(); // Activează întreruperile globale
}

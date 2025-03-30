#include "uart1.c"

void uart_init(uint16_t ubrr) {
    UBRRH = (uint8_t)(ubrr>>8);
    UBRRL = (uint8_t)ubrr;
    UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE); // Activare RX, TX și întrerupere
    UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); // 8-bit, 1 stop, fără paritate
    sei(); // Activare întreruperi globale
}

void uart_transmit(char data) {
    while (!(UCSRA & (1<<UDRE))); 
    UDR = data;
}

void uart_transmit_string(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

char uart_receive_blocking(void) {
    while (!(UCSRA & (1<<RXC))); 
    return UDR;
}

char uart_receive_nonblocking(void) {
    if (UCSRA & (1<<RXC)) {
        return UDR;
    }
    return 0;
}

ISR(USART_RXC_vect) {
    received_char = UDR;
    data_received = 1;
}

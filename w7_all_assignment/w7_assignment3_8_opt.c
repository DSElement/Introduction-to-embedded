#include "uart1.h"

 char received_char = 0;
 uint8_t data_received = 0;

ISR(USART_RXC_vect) {
    received_char = UDR;
    data_received = 1;
}

int main(void) {
    uart_init(MYUBRR);
    
    uart_transmit_string("UART Initialized\n");
    
    uint32_t counter = 0;
    char text[50];
    
    while (1) {
        snprintf(text, sizeof(text), "Counter: %lu\n", counter++);
        uart_transmit_string(text);
        
        if (data_received) {
            uart_transmit(received_char); 
            data_received = 0;
        }
        
        _delay_ms(1000);
    }
    return 0;
}

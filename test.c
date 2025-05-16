#include <lpc214x.h>

void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms * 6000; i++);
}

void UART0_Init(void) {
    PINSEL0 |= 0x00000005;  // Enable UART0 on P0.0 (TXD) and P0.1 (RXD)
    U0LCR = 0x83;           // DLAB = 1, 8-bit data, 1 stop bit, no parity
    U0DLL = 97;             // Assuming 15 MHz PCLK -> 9600 Baud
    U0DLM = 0;
    U0LCR = 0x03;           // DLAB = 0
    U0FCR = 0x07;           // Enable and reset TX and RX FIFO
}

void UART0_TxByte(unsigned char byte) {
    while (!(U0LSR & 0x20));  // Wait for THR empty
    U0THR = byte;
}

unsigned char UART0_RxByte(void) {
    while (!(U0LSR & 0x01));  // Wait for data
    return U0RBR;
}

void UART0_TxChar(char ch) {
    while (!(U0LSR & 0x20));  // Wait until THR is empty
    U0THR = ch;
}

void UART0_TxString(const char *str) {
    while (*str) {
        UART0_TxChar(*str++);
    }
}

int main(void) {
    UART0_Init();

	  UART0_TxByte('A');
	  UART0_TxString("UART ready\r\n");

    while (1) {
        // Wait for a byte from Docklight
        unsigned char received = UART0_RxByte();

        // For example: echo back + 1 (simple transformation)
        UART0_TxByte(received);
    }
}

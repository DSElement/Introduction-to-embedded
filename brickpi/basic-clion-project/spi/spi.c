//
// Created by Dan G on 6/18/2025.
//

#include "spi.h"
#include "lpc214x.h"

#define SPI_CS_PIN (1 << 7)  // P0.7 as software-controlled SSEL

void spi_init(void) {
    // Set P0.4 = SCK, P0.5 = MISO, P0.6 = MOSI, P0.7 = GPIO for CS
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);  // SPI function on P0.4–P0.6
    PINSEL0 &= ~((1 << 14) | (1 << 15));          // P0.7 as GPIO

    IO0DIR |= SPI_CS_PIN;  // Set P0.7 (CS) as output
    IO0SET = SPI_CS_PIN;   // Set CS high (inactive)

    S0SPCR = (1 << 5);      // Master mode, 8-bit, CPOL=0, CPHA=0
    S0SPCCR = 8;            // SPI clock = PCLK / 8
}

void spi_start(void) {
    IO0CLR = SPI_CS_PIN;  // Pull Chip Select low
}

void spi_stop(void) {
    IO0SET = SPI_CS_PIN;  // Pull CS high
}

uint8_t spi_transfer(uint8_t data) {
    S0SPDR = data;
    while (!(S0SPSR & (1 << 7)));  // Wait for transfer complete (SPIF)
    return S0SPDR;
}

void spi_transfer_bytes(uint8_t *tx_buf, uint8_t *rx_buf, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        S0SPDR = tx_buf[i];
        while (!(S0SPSR & (1 << 7)));
        if (rx_buf) {
            rx_buf[i] = S0SPDR;
        }
    }
}

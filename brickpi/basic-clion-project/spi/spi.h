//
// Created by Dan G on 6/18/2025.
//

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

	//S0SPCR (SPI0 Control Register)
	//S0SPSR (SPI0 Status Register)
	//S0SPDR (SPI Data Register)
	//S0SPCCR (SPI0 Clock Counter Register)
	//S0SPINT (SPI0 Interrupt Register)

// Initializes SPI0 in Master mode
void spi_init(void);

// Begins an SPI transfer (pulls CS low)
void spi_start(void);

// Ends an SPI transfer (pulls CS high)
void spi_stop(void);

// Sends and receives one byte
uint8_t spi_transfer(uint8_t data);

// Sends multiple bytes and reads the response
void spi_transfer_bytes(uint8_t *tx_buf, uint8_t *rx_buf, uint8_t len);

#endif // SPI_H

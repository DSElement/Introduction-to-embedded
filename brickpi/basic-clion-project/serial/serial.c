//
// Created by valy on 11/22/24.
//

#include "serial.h"
#include <stdint.h>
#include <lpc22xx.h>

int sendchar (int ch)
{
	/* Write character to Serial Port    */
	while (!(U0LSR & 0x20));
	return (U0THR = ch);
}

void xSerialPortInitMinimal(uint32_t baudrate, uint32_t queue_size)
{
	PINSEL0=0X5;
	U0LCR=0X83;
	U0DLL=98;
	U0DLM=0;
	U0LCR=0X03;
}

void vSerialPutString(void *serial, uint8_t* buff, uint32_t len)
{
	for (int i = 0; i < len; i++)
	{
		sendchar(buff[i]);
	}
}
/*
  ADC in LPC2148(ARM7)
  http://www.electronicwings.com/arm7/lpc2148-adc-analog-to-digital-converter
*/

#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void ser_int()
{
  PINSEL0=0X5;
  U0LCR=0X83;
  U0DLL=98;
  U0DLM=0;
  U0LCR=0X03;
}

void tx(unsigned char c)
{
  while((U0LSR&(1<<5))==0);
  U0THR=c;
}

void send_string(const char *s)
{
  while (*s) {
    tx(*s++);
  }
}

char rx()
{
  while( (U0LSR & 0x01) == 0);	/*Wait till RDR bit becomes 1 which tells that receiver contains valid data */
	return U0RBR;
}

volatile int i = 0;

int main(void)
{
	uint32_t result;
	float voltage;
	char volt[18];
	ser_int();
	PINSEL1 = 0x01000000; // P0.28 as AD0.1
	AD0CR = 0x00200402; // ADC operational, 10-bits, 11 clocks for conversion
	while(1)
	{
		AD0CR = AD0CR | (1<<24); // Start Conversion
		while ( !(AD0DR1 & 0x80000000) ); // Wait till DONE
		result = AD0DR1;
		result = (result>>6);
		result = (result & 0x000003FF);
		voltage = ( (result/1023.0) * 3.3 ); // Convert ADC value to equivalent voltage
		sprintf(volt, "Voltage=%.2f V  \n", voltage);
		send_string(volt);
		memset(volt, 0, 18);
		for (i = 0; i<1000000; i++);
	}
}
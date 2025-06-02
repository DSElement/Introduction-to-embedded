#include<lpc214x.h>

/*void pll()
{
  PLL0CON=0X01;
  PLL0CFG=0X24;
  PLL0FEED=0XAA;
  PLL0FEED=0X55;
  while((PLL0STAT&(1<<10))==0);
  PLL0CON=0X03;
  PLL0FEED=0XAA;
  PLL0FEED=0X55;
  VPBDIV=0x01;             //pclk=60mhz
}*/

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

int main(void)
{
  unsigned char b;
  ser_int();
  send_string("UART Ready!\r\n");  // Send greeting at startup
  while(1) {
    b = rx();
		tx(b);
  }
}

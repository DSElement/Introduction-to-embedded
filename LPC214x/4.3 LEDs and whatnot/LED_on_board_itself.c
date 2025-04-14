#include <lpc214x.h>
//quartz 12MHz

// CCLK = 12MHz * 5 = 60 MHz
// PCLK = 60 / 4 = 15
int main(void)
{
	volatile unsigned int i = 0;
	PINSEL1 &= ~((1u << 30) | (1u << 31));
	IO0DIR |= 1u << 31;
	IO1DIR |= (1u << 24);
	while(1)
	{ 
		// P0.31
		IO0SET = 1u << 31;
		IO1CLR = 1u << 24;
		for (i = 0; i < 1000000; i++);
		IO0CLR = 1u << 31;
		IO1SET = 1u << 24;
		for (i = 0; i < 1000000; i++);
	}
	
	return 0;
}
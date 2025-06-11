#include <log.h>
#include "lpc214x.h"
#include <stdio.h>
#include <stdint.h>
void my_delay();

void timer_init()
{
	T0MR0 = 1200000UL;
	T0MCR = 3;
	T0TCR = 1;
}

__attribute__((interrupt("FIQ"))) void my_interrupt(void)
{
	static volatile uint32_t n = 0;
	if (n % 4)
	{
		IO0SET = 1u << 31;
	}
	else
	{
		IO0CLR = 1u << 31;
	}
	n++;
	T0IR |= 1;
	VICVectAddr = 0;
}


void init_interrupt(void)
{
	VICIntEnable |= 1 << 4;
	VICIntSelect &= ~(1 << 4);
//	VICVectAddr0 = (int32_t)my_interrupt;
	VICVectCntl0 = 4 | ( 1 << 5);
}

void init_interrupt_fiq(void)
{
	VICIntEnable |= 1 << 4;
	VICIntSelect |= 1 << 4;
}

void set_pll(void)
{
	PLL0CON = 0x01;
	PLL0CFG = 0x23;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while ((PLL0STAT & (1 << 10)) == 0);
	PLL0CON = 0x03;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	VPBDIV = 0;
}

int main(void)
{
	volatile unsigned int i = 0;
	volatile unsigned int j = 0;
	volatile int a = 4;
	set_pll();
	char buffer[1000];
	PINSEL1 &= ~((1 << 29) | ( 1 << 28 ));
	IO0DIR |= 1 << 31; //P0.30 output

	timer_init();
	LOG_init();
	//init_interrupt();
	init_interrupt_fiq();
	while (1);
	while(1)
	{

		if (T0IR & (1 << 0))
		{
			a++;
			if (j % 2)
			{
				IO0SET = 1 << 30;
			}
			else
			{
				IO0CLR = 1 << 30;
			}
			j++;
			LOG_SERIAL("test %d\n", j);
			T0IR |= 1;
		}
	}
	while(1)
	{
		IO0SET |= 1 << 30;
		for (j = 0; j < 10000; j++);
		IO0CLR |= 1 << 30;
		for (j = 0; j < 10000; j++);
	}
	return 0;
}



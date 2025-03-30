/*
 * GccApplication4.c
 *
 * Created: 1/13/2025 3:02:41 PM
 * Author : student
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include "timer.c"


int main(void)	
{		
 
  sei();
 
  DDRB |= (1<<0);
 
  unsigned value = 0x7080;
  timer_init(value);
 
 while(1)
 {
 if ((TIFR & (1 << OCF1A)) != 0)
 {
	TCNT1=0;
	TIFR |= 1 << OCF1A; // reset flag
 }
 }
 // some code
} 
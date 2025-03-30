/*
 * CFile1.c
 *
 * Created: 1/13/2025 3:30:53 PM
 *  Author: student
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void timer_init(unsigned value)
{
	// some code
	//unsigned int value = 0x7080;
	// some code
	OCR1AH = (value >> 8) & 0xFF;
	OCR1AL = (value & 0xFF);
	// some code
	
	//TCCR1A = 0;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12);

	TIMSK |= 1<< OCIE1A;

}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << 0);
}

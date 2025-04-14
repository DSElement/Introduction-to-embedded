#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define TIMER_COMPARE_MATCH_FLAG (1 << OCF1A)

uint8_t counter = 0; // Define the volatile flag

void timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12);

	unsigned int compare_value = 57600; // Assuming 16MHz clock: (16MHz / (8 * 2Hz)) - 1
	OCR1AH = (compare_value >> 8) & 0xFF;
	OCR1AL = compare_value & 0xFF;

	TIMSK |= (1 << OCIE1A);
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	counter++;
	if (counter > 9)
		counter = 0;
	display_digit(counter);
}

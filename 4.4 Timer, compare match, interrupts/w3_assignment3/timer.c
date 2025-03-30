#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define TIMER_COMPARE_MATCH_FLAG (1 << OCF1A) 

uint8_t timer_flag = 0; // Define the volatile flag

void timer_init(void)
{
    TCCR1B |= (1 << WGM12); 
    TCCR1B |= (1 << CS11);  

    unsigned int compare_value = 62500; // Assuming 16MHz clock: (16MHz / (8 * 2Hz)) - 1
    OCR1AH = (compare_value >> 8) & 0xFF;
    OCR1AL = compare_value & 0xFF;

    TIMSK |= (1 << OCIE1A);  
    sei();                   
}

ISR(TIMER1_COMPA_vect)
{
    timer_flag = 1; // Set the flag when the interrupt occurs
}

#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "seven_segment.h"
#include "timer.c"

uint8_t counter = 0;  
uint8_t update_display = 0; 

ISR(TIMER1_COMPA_vect) {
    counter++;
    if (counter > 9) counter = 0;
    update_display = 1; 
}

int main(void) {
    segment_init();
    timer_init();
    sei(); 

    while (1) {
        if (update_display) {
            segment_display_digit(counter);
            update_display = 0; 
        }
    }
}

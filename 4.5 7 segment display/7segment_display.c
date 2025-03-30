#include "7segment_display.h"
#include <avr/io.h>
#include <util/delay.h>

void init_digit(void) {
    SEGMENT_DDR = 0xFF; 
    SEGMENT_PORT = 0x00; 
}

// Function to display a digit on the 7-segment display
void display_digit(uint8_t digit) {
    if (digit < 10) {
        SEGMENT_PORT = digitmap[digit]; 
    }
}

void clear_digit(void) {
    SEGMENT_PORT = 0x00;  // Set PORTB to 0 (turn off all segments)
}

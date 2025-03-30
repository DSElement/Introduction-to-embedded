#ifndef _SEVENSEGMENT_DISPLAY_H_
#define _SEVENSEGMENT_DISPLAY_H_

#include <stdint.h>

void init_digit(void);          
void display_digit(uint8_t digit); 
void clear_digit(void);          

// Mapping of digits (0-9) to 7-segment encoding
static const uint8_t digitmap[10] = {
    0xC0,  // 0
    0xF9,  // 1
    0xA8,  // 2
    0xB0,  // 3
    0x99,  // 4
    0x92,  // 5
    0x82,  // 6
    0xF8,  // 7
    0x80,  // 8
    0x90   // 9
};

#define SEGMENT_PORT     PORTB   // Example: PORTB for the 7-segment display
#define SEGMENT_DDR      DDRB    // Example: DDRB for the 7-segment display

#endif // _SEVENSEGMENT_DISPLAY_H_

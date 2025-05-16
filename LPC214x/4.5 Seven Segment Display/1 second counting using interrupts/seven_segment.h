#ifndef _SEVENSEGMENT_H_
#define _SEVENSEGMENT_H_

#include <stdint.h>

// Initialize GPIO pins for the 7-segment display
void init_digit(void);

// Display a digit (0–9)
void display_digit(uint8_t digit);

// Turn off all segments
void clear_digit(void);

// You can redefine this based on your pin connection (P0.8–P0.15 in this case)
#define SEGMENT_MASK (0xFF << 8)

#endif // _SEVENSEGMENT_DISPLAY_H_

#include "seven_segment.h"
#include "LPC214x.h"

// Digit encoding for common cathode 7-segment display
static const uint8_t digitmap[10] = {
    0xC0,  // 0
    0xF9,  // 1
    0xA4,  // 2
    0xB0,  // 3
    0x99,  // 4
    0x92,  // 5
    0x82,  // 6
    0xF8,  // 7
    0x80,  // 8
    0x90   // 9
};

void init_digit(void) {
    // Set P0.8–P0.15 as GPIO and output
    PINSEL0 &= ~(0xFFFF << 16); // Clear bits for P0.8–P0.15
    IO0DIR  |= SEGMENT_MASK;    // Set them as outputs
    clear_digit();
}

void display_digit(uint8_t digit) {
    if (digit < 10) {
        IOCLR0 = SEGMENT_MASK;                       // Clear old value
        IOSET0 = (digitmap[digit] << 8) & SEGMENT_MASK; // Set new value
    }
}

void clear_digit(void) {
    IOCLR0 = SEGMENT_MASK;  // Turn off all segments
}

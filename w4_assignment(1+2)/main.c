#include "7segment_display.h"

int main(void) {
    init_digit(); 
    
    while (1) {
        display_digit(3); // Display the digit '3' on the 7-segment display
        _delay_ms(1000);  // Wait for 1 second
        
        clear_digit();    // Clear the display
        _delay_ms(1000);  // Wait for 1 second
    }
}

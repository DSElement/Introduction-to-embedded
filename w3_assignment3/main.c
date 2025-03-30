#include <avr/io.h>
#include "timer.h"

#define LED_PIN PB0

void setup(void)
{
    DDRB |= (1 << LED_PIN); // Set LED_PIN as output
    timer_init();           // Initialize Timer1
}

int main(void)
{
    setup();

    while (1)
    {
        if (timer_flag)
        {
            PORTB ^= (1 << LED_PIN); // Toggle LED state
            timer_flag = 0;          // Reset the flag
        }
    }

    return 0;
}
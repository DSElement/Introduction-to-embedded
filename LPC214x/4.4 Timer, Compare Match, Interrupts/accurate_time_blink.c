#include "LPC214x.h"

#define LED_PIN (1 << 15)


void timer0_init(unsigned int match_value)
{
    T0TCR = 0x02;             // Reset Timer
    T0PR  = 0;                // Prescaler = 0, every PCLK tick is counted
    T0MR0 = match_value;      // Match after match_value ticks
    T0MCR = (1 << 0) | (1 << 1); // Interrupt & Reset on MR0
    T0TCR = 0x01;             // Enable Timer
}

int main(void)
{
    // Set P0.15 as GPIO
    PINSEL0 &= ~(3 << 30);   // Bits 30-31 = 00 -> GPIO
    IO0DIR  |= LED_PIN;      // P0.15 output
	
		PINSEL0 &= ~(0xFF << 16);
		IO0DIR |= (0xFF << 8); 		//ardeau leduri in plus si le-am oprit pe toate :)
		IOCLR0 = (0xFF << 8);

    // Timer0 for 500ms delay (assuming 15MHz PCLK)
    timer0_init(7500000);

    while (1)
    {
        if (T0IR & 0x01)     // Match Register 0 interrupt flag
        {
            IO0PIN ^= LED_PIN; // Toggle LED
            T0IR = 0x01;       // Clear MR0 interrupt
        }
    }
}
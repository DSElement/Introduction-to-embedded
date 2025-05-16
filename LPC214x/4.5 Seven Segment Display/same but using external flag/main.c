#include "seven_segment.h"
#include "LPC214x.h"

volatile uint8_t counter = 0;
volatile uint8_t tick = 0;  // Flag set by ISR

void __irq TIMER0_IRQHandler(void)
{
    tick = 1;               // Set flag

    T0IR = 0x01;              // Clear MR0 interrupt
    VICVectAddr = 0x00;       // Signal end of interrupt
}

void timer0_init(unsigned int match_value)
{
    // Timer config
    T0TCR = 0x02;             // Reset Timer
    T0PR = 0;                 // No prescaler
    T0MR0 = match_value;      // Set match value
    T0MCR = 3;                // Interrupt & Reset on MR0
    T0TCR = 0x01;             // Enable Timer

    // VIC setup
    VICIntSelect &= ~(1 << 4);   // Timer0 = IRQ
    VICIntEnable |= (1 << 4);    // Enable Timer0 interrupt
    VICVectAddr4 = (unsigned)TIMER0_IRQHandler;
    VICVectCntl4 = 0x20 | 4;     // Enable slot 4, source 4 (Timer0)
}

int main(void) {
    init_digit();
	  display_digit(counter);

	
	  timer0_init(15000000);

    __enable_irq();  // Global interrupt enable

    while (1) {
			if (tick) {
            tick = 0;             // Reset the flag
            counter = (counter + 1) % 10;
            display_digit(counter);
			}
		}
}

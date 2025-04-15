#include "LPC214x.h"

#define LED_PIN (1 << 15)

void __irq TIMER0_IRQHandler(void)
{
    // Toggle LED
    IO0PIN ^= LED_PIN;

    T0IR = 0x01;              // Clear MR0 interrupt
    VICVectAddr = 0x00;       // Signal end of interrupt
}

void timer0_init(unsigned int match_value)
{
    // Configure P0.15 as output
    PINSEL0 &= ~(3 << 30);    // P0.15 = GPIO
    IO0DIR |= LED_PIN;

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

int main(void)
{
    // Optional: Ensure PCLK = 15MHz
    //VPBDIV = 0x01;

    // 500ms @ 15MHz = 7,500,000
    timer0_init(7500000);

    __enable_irq();  // Global interrupt enable

    while (1)
    {
        // Interrupt handles everything
    }
}
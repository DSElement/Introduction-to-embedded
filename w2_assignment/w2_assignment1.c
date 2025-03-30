
#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>




int main(void)
{
    DDRB |= (1 << 0);
    while (1) 
    {
		PORTB |= (1 << 0);
		_delay_ms(50);
		PORTB &= ~(1 << 0);
		_delay_ms(50);
    }
}


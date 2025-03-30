/*
 * GccApplication2.c
 *
 * Created: 16.12.2024 14:15:55
 * Author : dspproject
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>




int main(void)
{
    DDRB |= (1 << 0);
	DDRA &= ~(1 << 6);
	
	//PORTB &= ~(1 << 0);
	while (1){
		if ((PINA & (1 << 6)) == 0){
			_delay_ms(1);
			if ((PINA & (1 << 6)) == 0){
				if ((PORTB & (1 << 0)) == 1){
					PORTB &= ~(1 << 0);
				}
				else {
					PORTB |= (1 << 0);
				}				
			}
		} 
	}
}


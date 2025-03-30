#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA &= ~((1 << 0) | (1 << 1) ) ; //buton
	DDRB = 0xFF; //led-uri
	
	//PORTB = 0;
	PORTB |= (1 << 0); // aprindem led-ul 0
	uint8_t current_led = 0; //sa stim unde suntem
	while (1){
		if ((PINA & (1 << 0)) == 0){
			_delay_ms(50);
			if ((PINA & (1 << 0)) == 0){
				current_led = (current_led == 0) ? 7 : (current_led - 1);
				PORTB = (1 << current_led);
			}
		}
		
		if ((PINA & (1 << 1)) == 0){
			_delay_ms(50);
			if ((PINA & (1 << 1)) == 0){
				current_led = (current_led == 7) ? 0 : (current_led + 1);
				PORTB = (1 << current_led);
			}
		}
		_delay_ms(100);
	}
}
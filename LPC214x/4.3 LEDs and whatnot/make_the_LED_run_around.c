#include <lpc214x.h>

#define OFFSET 8

int main(void){
	volatile unsigned int i = 0, current_led = OFFSET;
	PINSEL0 &= ~((1u << 0) | (1u << 1));  //p0.00 setat ca GPIO
	PINSEL0 &= ~((1u << 2) | (1u << 3));	//p0.01 setat ca GPIO
	IO0DIR &= ~((1u << 0) | (1u << 1));		//p0.00 si p0.01 setate ca input
	
	/*PINSEL0 &= ~((1u << 16) | (1u << 17));
	PINSEL0 &= ~((1u << 18) | (1u << 19));
	PINSEL0 &= ~((1u << 20) | (1u << 21));
	PINSEL0 &= ~((1u << 22) | (1u << 23));
	PINSEL0 &= ~((1u << 24) | (1u << 25));
	PINSEL0 &= ~((1u << 26) | (1u << 27));
	PINSEL0 &= ~((1u << 28) | (1u << 29));
	PINSEL0 &= ~((1u << 30) | (1u << 31));*/
	
	PINSEL0 &= ~(0xFF << 2*OFFSET);
	
	IO0DIR |= (0xFF << OFFSET); //punem 8 fire ca output
	
	IOSET0 = (1u << OFFSET); // aprindem un led
	
	while (1){
		if (!(IO0PIN & (1u << 0))){
			for (i = 0; i<100000; i++);
			if (!(IO0PIN & (1u << 0))){
				current_led = (current_led == OFFSET) ? (OFFSET+7) : (current_led - 1);
				IOCLR0 = (0xFF << OFFSET);
				IOSET0 = (1u << current_led);
				while (!(IO0PIN & (1u << 0)));
			}
		}
		
		if (!(IO0PIN & (1u << 1))){
			for (i = 0; i<100000; i++);
			if (!(IO0PIN & (1u << 1))){
				current_led = (current_led == OFFSET+7) ? (OFFSET) : (current_led + 1);
				IOCLR0 = (0xFF << OFFSET);
				IOSET0 = (1u << current_led);
				while (!(IO0PIN & (1u << 1)));
			}
		}
	}		
	return 0;
}
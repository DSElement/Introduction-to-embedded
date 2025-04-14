#include <lpc214x.h>

int main(void){
	volatile unsigned int i = 0;
	PINSEL0 &= ~((1u << 0) | (1u << 1));
	PINSEL0 &= ~((1u << 6) | (1u << 7));
	IO0DIR |= (1u << 0);
	IO0DIR &= ~(1u << 3);
	//p0.00
	while(1){
		if (!(IO0PIN & (1u << 3))){
			for (i = 0; i < 100000; i++);
			if (!(IO0PIN & (1u << 3))){
				if (IO0PIN & (1u << 0)){
					IO0CLR = 1u << 0;
				}	
				else {
					IO0SET = 1u << 0;
				}
			}
			while (!(IO0PIN & (1u << 3)));
		}
	}
	return 0;
}
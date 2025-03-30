#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "7segment_display.c"
#include "timer.c"

int main(void) {
	timer_init();
	init_digit();
	while (1) {
		
	}
}

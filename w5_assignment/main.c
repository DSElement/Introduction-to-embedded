#define F_CPU 14745600UL

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main(void) {
    lcd_init();
    lcd_clear();

    lcd_set_cursor(0, 0);

    while (1) {
        lcd_set_cursor(1, 0);
        lcd_print("Counter: ");
        for (int i = 0; i < 100; i++) {
            lcd_set_cursor(1, 9);
            char buf[5];
            itoa(i, buf, 10);
            lcd_print(buf);
            _delay_ms(500);
        }
    }
}

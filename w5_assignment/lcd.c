#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT 
#define LCD_DDR  
#define RS 0
#define E  1

void lcd_command(uint8_t cmd) {
    LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); 
    LCD_PORT &= ~(1 << RS); 
    LCD_PORT |= (1 << E); 
    _delay_us(1);
    LCD_PORT &= ~(1 << E);

    _delay_us(200);

    LCD_PORT = (LCD_PORT & 0x0F) | (cmd << 4);
    LCD_PORT |= (1 << E);
    _delay_us(1);
    LCD_PORT &= ~(1 << E);

    _delay_ms(2);
}

void lcd_init(void) {
    LCD_DDR = 0xFF;
    _delay_ms(15);

    lcd_command(0x33); 
    lcd_command(0x32); 
    lcd_command(0x28); 
    lcd_command(0x0C); 
    lcd_command(0x06); 
    lcd_command(0x01); 
    _delay_ms(2);
}

void lcd_clear(void) {
    lcd_command(0x01); 
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x80 : 0xC0;
    addr += col;
    lcd_command(addr);
}

void lcd_print(const char *str) {
    while (*str) {
        LCD_PORT = (LCD_PORT & 0x0F) | (*str & 0xF0);
        LCD_PORT |= (1 << RS);
        LCD_PORT |= (1 << E);
        _delay_us(1);
        LCD_PORT &= ~(1 << E);

        _delay_us(200);

        LCD_PORT = (LCD_PORT & 0x0F) | (*str << 4);
        LCD_PORT |= (1 << E);
        _delay_us(1);
        LCD_PORT &= ~(1 << E);

        _delay_ms(2);
        str++;
    }
}

#ifndef _LCD_H
#define _LCD_H

void lcd_clear(void);
void lcd_write(uint8_t, uint8_t *);
void lcd_init(void);

void lcd_home(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);

#endif
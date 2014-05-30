#include "hal.h"
#include "lcd.h"

#ifndef _LCD_C
#define _LCD_C

/*
 * Clock pulse
 */
static void lcd_nybble(void) {
  LCD_E(ON);
  /* Longer delay for long wires and breadboard */
  delay_ms(5);  /* Enable pulse width >= 300ns */
  LCD_E(OFF);   /* Clock enable: falling edge */
}

/*
 * Send a command
 */
void lcd_cmd(uint8_t cmd) {
  LCD_DB(cmd);                  /* Put data on output Port */
  LCD_RS(0);                    /* D/I=LOW : send instruction */
  LCD_RW(0);                    /* R/W=LOW : Write */
  lcd_nybble();                 /* Send upper 4 bits */
  cmd = (uint8_t)(cmd << 4);    /* Shift over by 4 bits */
  LCD_DB(cmd);                  /* Put data on output Port */
  lcd_nybble();                 /* Send lower 4 bits */
}

/*
 * Send 8-bit data
 */
void lcd_data(uint8_t cmd) {
  LCD_DB(cmd);                  /* Put data on output Port */
  LCD_RS(1);                    /* D/I=LOW : send data */
  LCD_RW(0);                    /* R/W=LOW : Write */
  lcd_nybble();                 /* Send upper 4 bits */
  cmd = (uint8_t)(cmd << 4);    /* Shift over by 4 bits */
  LCD_DB(cmd);                  /* Put data on output Port */
  lcd_nybble();                 /* Send lower 4 bits */
}

/*
 * Clear the LCD
 */
void lcd_clear(void) {
  lcd_cmd(0x01);
}

/*
 * Return cursor to home
 */
void lcd_home(void) {
  lcd_cmd(0x02);
}

/*
 * Turn cursor on
 */
void lcd_cursor_on(void) {
  lcd_cmd(0x0E);
}

/*
 * Turn cursor off
 */
void lcd_cursor_off(void) {
  lcd_cmd(0x0C);
}

/*
 * Write a message to the LCD
 */
void lcd_write(uint8_t line, uint8_t *msg) {
  if (line == 1) {
    lcd_cmd(BIT7 | 0x00);
  } else {
    lcd_cmd(BIT7 | 0x40);
  }
  while (*msg) {
    lcd_data(*msg++);
  }
}

/*
 * Initialize LCD in 4-bit mode
 */
void lcd_init(void) {
  LCD_E(0);
  delay_ms(50);         /* Wait >40 msecs after power is applied */
  LCD_DB(0x30);         /* Put 0x30 on the output port */
  delay_ms(5);          /* Must wait 5ms, busy flag not available */
  lcd_nybble();         /* Command 0x30 = Wake up */
  delay_ms(1);          /* Must wait 160us, busy flag not available */
  lcd_nybble();         /* Command 0x30 = Wake up #2 */
  delay_ms(1);             /* Must wait 160us, busy flag not available */
  lcd_nybble();         /* Command 0x30 = Wake up #3 */
  delay_ms(1);             /* Can check busy flag now instead of delay */
  LCD_DB(0x20);         /* Put 0x20 on the output port */
  lcd_nybble();         /* Function set: 4-bit interface */
  lcd_cmd(0x28);    /* Function set: 4-bit/2-line */
  lcd_cmd(0x10);    /* Set cursor */
  lcd_cmd(0x0F);    /* Display ON; Blinking cursor */
  lcd_cmd(0x06);    /* Entry Mode set */
}

#endif
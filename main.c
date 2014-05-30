#include <stdint.h>
#include "hal.h"
#include "lcd.h"

/* Register to handle up to 8 interrupt flags */
volatile uint8_t interrupt_flags;
/* Flag for game */
volatile uint8_t go_flag;

void main(void) {
  stop_wdt();
  
  /* Set up the ports for the system */
  port_config();
  
  /* Initialize the LCD */
  lcd_init();
  lcd_clear();
  /* Write a message to show game beginning */
  lcd_write(1, "Reflex");
  lcd_write(2, "game!");
  lcd_cursor_off();
  delay_ms(2000);
  
  while (1) {
    lcd_clear();
    lcd_write(1, "Ready?");
    
    interrupt_flags = 0;
    go_flag = 0;
    start_round();
    enable_interrupts();
    LPM;                /* Wait for interrupt from button or timer */
    if (go_flag) {      /* Woken up by timer */
      lcd_write(2, "GO!");
      LPM;              /* Wait for a player to press a button */
    }
    disable_interrupts();
        
    uint8_t flags = interrupt_flags;
    /* Handle button 1 press */
    if (flags & BUTTON1_FLAG) {
        lcd_clear();
        lcd_write(1, "Player 1");
      if (go_flag) {    /* Player can only win if it's GO! time */
        lcd_write(2, "wins!");
      } else {          /* Otherwise, they lose */
        lcd_write(2, "loses :(");
      }
      /* Debouncing */
      delay_ms(10);
      /* Wait for button release */
      while (BUTTON1);
      /* Debouncing */
      delay_ms(10);
      interrupt_flags &= ~BUTTON1_FLAG;
    } else if (flags & BUTTON2_FLAG) {
        lcd_clear();
        lcd_write(1, "Player 2");
      if (go_flag) {    /* Player can only win if it's GO! time */
        lcd_write(2, "wins!");
      } else {          /* Otherwise, they lose */
        lcd_write(2, "loses :(");
      }
      /* Debouncing */
      delay_ms(10);
      /* Wait for button release */
      while (BUTTON2);
      /* Debouncing */
      delay_ms(10);
      interrupt_flags &= ~BUTTON2_FLAG;
    }
    
    /* Show winner for 2 seconds before starting next round */
    delay_ms(2000);
  }
}

#pragma vector = TIMER_VECTOR
__interrupt void TIMER_ISR(void) {
  if (TIMER_IFG) {
    TIMER_CLEAR_IFG();
    TIMER_STOP();
    go_flag = 1;
    LPM_EXIT;
  }
}

#pragma vector = BUTTON_VECTOR
__interrupt void BUTTON_ISR(void) {
  /* Button 1 */
  if (BUTTON1_IFG) {
    interrupt_flags |= BUTTON1_FLAG;
    BUTTON1_CLEAR_IFG();
  }
  /* Button 2 */
  if (BUTTON2_IFG) {
    interrupt_flags |= BUTTON2_FLAG;
    BUTTON2_CLEAR_IFG();
  }
  LPM_EXIT;
}
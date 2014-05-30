#include <msp430g2553.h>
#include <stdint.h>

/* Prevent multiple inclusion */
#ifndef _HAL_H
#define _HAL_H

enum {
  OFF = 0,
  ON = 1,
};

enum {
  CLOCK_SPEED = 1, /* Clock speed in MHz */
};

enum button_info {
  BUTTON1_BIT = BIT1,
  BUTTON2_BIT = BIT2,
  BUTTON_VECTOR = PORT2_VECTOR,
  BUTTON1_FLAG = BIT0,
  BUTTON2_FLAG = BIT1,
};

enum timer_info {
  TIMER_VECTOR = TIMER0_A1_VECTOR,
};

#define LPM                     LPM1
#define LPM_EXIT                LPM1_EXIT

/* LCD */
#define LCD_E(B) ((B == 0) ? (P2OUT &= ~BIT0) : (P2OUT |= BIT0))
#define LCD_RS(B) ((B == 0) ? (P1OUT &= ~BIT4) : (P1OUT |= BIT4))
#define LCD_RW(B) ((B == 0) ? (P1OUT &= ~BIT5) : (P1OUT |= BIT5))
#define LCD_DB4(B) ((B == 0) ? (P1OUT &= ~BIT0) : (P1OUT |= BIT0))
#define LCD_DB5(B) ((B == 0) ? (P1OUT &= ~BIT1) : (P1OUT |= BIT1))
#define LCD_DB6(B) ((B == 0) ? (P1OUT &= ~BIT2) : (P1OUT |= BIT2))
#define LCD_DB7(B) ((B == 0) ? (P1OUT &= ~BIT3) : (P1OUT |= BIT3))
#define LCD_DB(B) (LCD_DB4((uint8_t)(B & BIT4)), \
                   LCD_DB5((uint8_t)(B & BIT5)), \
                   LCD_DB6((uint8_t)(B & BIT6)), \
                   LCD_DB7((uint8_t)(B & BIT7)))

#define BUTTON1                 (P2IN & BUTTON1_BIT)
#define BUTTON2                 (P2IN & BUTTON2_BIT)
#define BUTTON1_IFG             (P2IFG & BUTTON1_BIT)
#define BUTTON1_PRESSED         (P2IN & BUTTON1_BIT)
#define BUTTON1_CLEAR_IFG()     (P2IFG &= ~BUTTON1_BIT)
#define BUTTON2_IFG             (P2IFG & BUTTON2_BIT)
#define BUTTON2_PRESSED         (P2IN & BUTTON2_BIT)
#define BUTTON2_CLEAR_IFG()     (P2IFG &= ~BUTTON2_BIT)

#define TIMER_IFG               (TAIV == TA0IV_TAIFG)
#define TIMER_CLEAR_IFG()       (TACTL &= ~TAIFG)
#define TIMER_STOP()            (TACTL = MC_0)

#define enable_interrupts()     __enable_interrupt()
#define disable_interrupts()    __disable_interrupt()

void stop_wdt(void);
void delay_ms(uint16_t);
void port_config(void);
void start_round(void);

#endif
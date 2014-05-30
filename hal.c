#include <msp430g2553.h>
#include <stdint.h>
#include "hal.h"

/* Prevent multiple inclusion */
#ifndef _HAL_C
#define _HAL_C

/*
 * Stop the watchdog timer
 */
void stop_wdt(void) {
  /* Disable watchdog timer */
  WDTCTL = WDTPW | WDTHOLD;
}

/*
 * Do nothing for a specified number of milliseconds
 */
void delay_ms(uint16_t ms) {
  while (ms--) __delay_cycles(CLOCK_SPEED * 1000);
}

/*
 * Configure I/O ports
 */
void port_config(void) {
  /* Set P1.0, P1.1, P1.2, and P1.3 as data outputs for the display */
  P1DIR |= BIT0 | BIT1 | BIT2 | BIT3;
  /* Set P1.4 as output for RS, P1.5 as output for R/W */
  P1DIR |= BIT4 | BIT5;
  /* Set P2.0 as output for E(timer) */
  P2DIR |= BIT0;
  
  /* Set P2.1 to 0 as input for button (S3) */
  P2DIR &= ~BIT1;
  /* Enable interrupt on P2.1 */
  P2IE |= BIT1;
  /* Interrupt flag low-to-high (rising edge) transition on P2.1 */
  P2IES &= ~BIT1;
  /* Set P2.2 to 0 as input for button (S4) */
  P2DIR &= ~BIT2;
  /* Enable interrupt on P2.2 */
  P2IE |= BIT2;
  /* Interrupt flag low-to-high (rising edge) transition on P2.2 */
  P2IES &= ~BIT2;
}

/*
 * Start the timer for the round, testing the player's reflexes
 */
void start_round(void) {
  /* Configure Timer_A for ending after 7 seconds */
  /* ACLK source (12 kHz), /8 divider, up mode, enable_interrupts, clear */
  TACTL = TASSEL_1 | ID_3 | MC_1 | TAIE | TACLR;
  /* Clock source source frequency = 12 kHz / 8 = 1500 Hz */
  TACCR0 = 10500; /* 1500 * 7 = 10500 */
}

#endif
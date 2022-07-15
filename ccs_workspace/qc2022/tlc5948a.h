/*
 * leds.h
 * (c) 2014 George Louthan
 * 3-clause BSD license; see license.md.
 */

#ifndef TLC5948A_H_
#define TLC5948A_H_

#include <stdint.h>

// Defines for pins:

// Defines for the TLC:
#define TLC_LATPORT GPIO_PORT_P1
#define TLC_LATPIN  GPIO_PIN7

#define LAT_POUT    P1OUT
#define LAT_PBIT    BIT7

// LED sending types:
#define TLC_SEND_IDLE     0
#define TLC_SEND_TYPE_GS  1
#define TLC_SEND_TYPE_FUN 2
#define TLC_SEND_TYPE_LB  3

void tlc_init();

uint8_t tlc_test_loopback(uint8_t);

void tlc_set_gs();
void tlc_set_fun();
void tlc_stage_blank(uint8_t);

extern uint8_t tlc_send_type;
extern uint16_t tlc_bank_gs[6][16];

#endif /* TLC5948A_H_ */

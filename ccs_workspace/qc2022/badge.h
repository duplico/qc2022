/*
 * badge.h
 *
 *  Created on: Jun 26, 2022
 *      Author: george
 */

#ifndef BADGE_H_
#define BADGE_H_

// SMCLK rate in Hz:
#define SMCLK_RATE_HZ 8000000

// Dot correction configuration for the three colors:
#define TLC_DC_RED 0x20
#define TLC_DC_GRN 0x19
#define TLC_DC_BLU 0x2a

// Global brightness correct:
#define TLC_BC 0x00 // 25%

extern volatile uint8_t f_time_loop;

#endif /* BADGE_H_ */

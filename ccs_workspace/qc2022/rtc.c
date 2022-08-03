/*
 * rtc.c
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#include <stdint.h>

#include <msp430fr2633.h>

#include "badge.h"

extern uint8_t button_state;
uint8_t rtc_button_csecs = 0;
volatile uint8_t rtc_centiseconds = 0;
volatile uint32_t rtc_seconds = 0;

/// Initialize the on-board real-time clock to tick 100 times per second.
/**
 ** This sources the RTC from SMCLK (8 MHz) divided by 1000 (8 kHz),
 ** and setting the modulo to 80, so that the RTC will tick 100x
 ** per second.
 */
void rtc_init() {
    rtc_seconds = badge_conf.clock;

    RTCMOD = 80; // Count the clock to 80 before resetting.

    // Read and then throw away RTCIV to clear the interrupt.
    volatile uint16_t vector_read;
    vector_read = RTCIV;

    RTCCTL = RTCSS__SMCLK |     // SMCLK (8MHz) source
             RTCPS__1000 |      // divided by 1000 to get 8kHz
             RTCSR |            // Reset counter.
             RTCIE;             // Enable interrupt.
}

// The clock is measured in seconds since noon Vegas time on Wednesday.
// So, here are some example times:
//  Seconds     Real time
//  -------     ---------
//  0           Noon Wednesday
//  43200       Midnight Thursday morning
//  86400       Noon Thursday
//  129600      Midnight Friday morning
//  172800      Noon Friday
//  212400      11pm Friday (party!)
//  216000      Midnight Saturday morning

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void) {
    // Called when the RTC overflows (100 times per second)
    if (RTCIV == RTCIV_RTCIF) {
        rtc_centiseconds++;
        f_time_loop = 1;

        if (button_state && rtc_centiseconds == rtc_button_csecs) {
            f_long_press = 1;
        }

        if (rtc_centiseconds == 100) {
            rtc_centiseconds = 1;
            rtc_seconds++;

            if (!(rtc_seconds % BADGE_CLOCK_WRITE_INTERVAL)) {
                // Every BADGE_CLOCK_WRITE_INTERVAL seconds, write our time to the config.
                badge_set_time(rtc_seconds, badge_conf.clock_authority);
            }

            if (!(rtc_seconds % BADGE_BLING_SECS)) {
                f_bling = 1;
            }
        }

        LPM0_EXIT;
    }
}

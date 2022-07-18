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

uint32_t rtc_seconds = 0;

/// Initialize the on-board real-time clock to tick once per second.
/**
 ** This sources the RTC from SMCLK (8 MHz) divided by 1000 (8 kHz),
 ** and setting the modulo to 8000, so that the RTC will tick once
 ** per second.
 */
void rtc_init() {
    RTCMOD = 8000; // Count the clock to 8000 before resetting.

    // Read and then throw away RTCIV to clear the interrupt.
    volatile uint16_t vector_read;
    vector_read = RTCIV;

    RTCCTL = RTCSS__SMCLK |     // SMCLK (8MHz) source
             RTCPS__1000 |      // divided by 1000 to get 8kHz
             RTCSR |            // Reset counter.
             RTCIE;             // Enable interrupt.
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void) {
    // Called when the RTC overflows (currently every second)
    if (RTCIV == RTCIV_RTCIF) {
        rtc_seconds++;
        if (button_state) {
            f_long_press = 1;
        }
    }
}

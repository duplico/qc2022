/*
 * rtc.h
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#ifndef RTC_H_
#define RTC_H_

/// Total number of seconds since badge activation.
extern volatile uint32_t rtc_seconds;
extern volatile uint8_t rtc_centiseconds;
extern uint8_t rtc_button_csecs;

void rtc_init();

#endif /* RTC_H_ */

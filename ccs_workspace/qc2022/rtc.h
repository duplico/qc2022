/*
 * rtc.h
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#ifndef RTC_H_
#define RTC_H_

/// Total number of seconds since badge activation.
extern uint32_t rtc_seconds;

void rtc_init();

#endif /* RTC_H_ */

//
// Created by codetector on 5/3/18.
//

#ifndef IN18_NIXIECLOCK_DS3231_H
#define IN18_NIXIECLOCK_DS3231_H

#include <stdint.h>

#define DS3231_I2C_Address 0b11010000

typedef struct rtcDateTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtcTime_t;

void RTC_setTime(rtcTime_t timeSet);

rtcTime_t RTC_offset(rtcTime_t time, int8_t offsetHour);
rtcTime_t RTC_readTime();

#endif //IN18_NIXIECLOCK_DS3231_H

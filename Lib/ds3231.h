//
// Created by codetector on 5/3/18.
//

#ifndef IN18_NIXIECLOCK_DS3231_H
#define IN18_NIXIECLOCK_DS3231_H

#define DS3231_I2C_Address 0b1101000

struct rtctime_t {
    uint8_t hour;
    uint8_t minutes;
    uint8_t second;
};

#endif //IN18_NIXIECLOCK_DS3231_H

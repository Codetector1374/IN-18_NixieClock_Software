//
// Created by codetector on 5/3/18.
//

#include "ds3231.h"
#include "nixieclock.h"
#include "stm32f1xx_hal.h"

uint8_t BCD2DEC(uint8_t data);
uint8_t HOUR_BCD2DEC(uint8_t data);
uint8_t DEC2BCD(uint8_t data);
uint8_t HOUR_DEC2BCD(uint8_t hour) ;

void RTC_init() {
    uint8_t data[0x13];
    uint8_t dataToSend = 0x0;

    HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_Address, &dataToSend, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_Address, data, 0x13, 50);

    if ((data[1] >> 7) & 0b1) {
        dataToSend = 0x0;
    }
}

rtcTime_t RTC_readTime() {
    rtcTime_t time = {.hour = 0, .minute = 0, .second = 0};

    uint8_t data[3];
    uint8_t dataToTransimit = 0x0;
    HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_Address, &dataToTransimit, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_Address, data, 3, 50);

    time.second = BCD2DEC(data[0]);
    time.minute = BCD2DEC(data[1]);
    time.hour = HOUR_BCD2DEC(data[2]);

    return time;
}

void RTC_setTime(rtcTime_t timeSet) {
    uint8_t data[4];
    data[0] = 0;
    data[1] = DEC2BCD(timeSet.second);
    data[2] = DEC2BCD(timeSet.minute);
    data[3] = HOUR_DEC2BCD(timeSet.hour);

    HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_Address, data, 4, 50);
}

rtcDate_t RTC_readDate() {
    uint8_t data[3];
    uint8_t address = 0x3;
    HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_Address, &address, 1, 50);
    HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_Address, data, 3, 50);

    rtcDate_t date = {.day = 0, .month = 0, .year = 0};

    date.day = BCD2DEC(data[0]);
    int centryFlag = (data[1] >> 7) & 0b1;
    date.month = BCD2DEC((data[1] & (uint8_t)0b01111111));
    uint16_t year = (uint16_t) BCD2DEC(data[2]);
    if (centryFlag) {
        year += 2000;
    } else {
        year += 1900;
    }
    date.year = year;
    return date;
}

rtcTime_t RTC_offset(rtcTime_t time, int8_t offsetHour) {
    rtcTime_t timeMod = time;
    timeMod.hour += offsetHour;
    if (timeMod.hour > 24) {
        timeMod.hour -= 24;
    }
    if (timeMod. hour < 0) {
        timeMod.hour += 24;
    }

    return timeMod;
}

uint8_t HOUR_BCD2DEC(uint8_t data) {
    uint8_t time = (uint8_t) (data & 0b1111);
    if (((data >> 4) & 1U)) {
        time +=10;
    }
    if ((data >> 5) & 1U) {
        if ((data >> 7) & 1U) { // 12 Hour Mode
            time += 12;
        } else {
            time += 20;
        }
    }
    return time;
}

uint8_t HOUR_DEC2BCD(uint8_t hour) {
    uint8_t output = (uint8_t) (hour % 10);

    if (hour >= 20) {
        output |= 0b00100000;
    } else if (hour >= 10) {
        output |= 0b00010000;
    }

    return  output;
}

uint8_t BCD2DEC(uint8_t data) {
    return (uint8_t) ((data >> 4) * 10 + (data & 0x0F));
}

uint8_t DEC2BCD(uint8_t data) {
    return (uint8_t) (((data / 10) << 4) | (data % 10));
}
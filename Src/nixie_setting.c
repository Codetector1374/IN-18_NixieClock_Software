//
// Created by codetector on 5/4/18.
//

#include "nixie_setting.h"
#include "nixieclock.h"
#include "ds3231.h"


uint16_t unsignedBoundedIncrement(uint16_t value, uint16_t max, int8_t change) ;

uint16_t readSettingValue(clockSettingItem_t item) {
    switch (item) {
        default:
            return current_setting_value;
    }
}

uint16_t readStoredSettingValue(clockSettingItem_t item) {
    switch (item) {
        case CLOCKSETTING_ANTIPOSION_INTERVAL:

        case CLOCKSETTING_HOUR:
            return RTC_readTime().hour;
        case CLOCKSETTING_MINUTE:
            return RTC_readTime().minute;
        case CLOCKSETTING_SECOND:
            return RTC_readTime().second;
        default:
            return readSettingValue(item);
    }
}

void updateSettingValue(clockSettingItem_t item, int8_t update) {
    switch (item) {
        case CLOCKSETTING_HOUR:
            current_setting_value = unsignedBoundedIncrement(current_setting_value, 23, update);
            break;
        case CLOCKSETTING_MINUTE:
        case CLOCKSETTING_SECOND:
            current_setting_value = unsignedBoundedIncrement(current_setting_value, 59, update);
            break;
    }
}

uint16_t unsignedBoundedIncrement(uint16_t value, uint16_t max, int8_t change) {
    if (change > 0) {
        if (value < max) {
            return ++value;
        } else {
            return 0;
        }
    }
    if (change < 0) {
        if (value > 0) {
            return --value;
        } else {
            return max;
        }
    }
}

void saveCurrentSettingIfNeeded(clockSettingItem_t item) {
    if (current_setting_changed) {
        rtcTime_t t = RTC_readTime();
        switch (item) {
            case CLOCKSETTING_HOUR:
                t.hour = (uint8_t) current_setting_value;
                RTC_setTime(t);
                break;
            case CLOCKSETTING_MINUTE:
                t.minute = (uint8_t) current_setting_value;
                RTC_setTime(t);
            case CLOCKSETTING_SECOND:
                t.second = (uint8_t) current_setting_value;
                RTC_setTime(t);
            default:
                break;
        }
    }
}
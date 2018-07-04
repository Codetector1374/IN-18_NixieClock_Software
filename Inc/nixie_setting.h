//
// Created by codetector on 5/4/18.
//

#ifndef IN18_NIXIECLOCK_NIXIE_SETTING_H
#define IN18_NIXIECLOCK_NIXIE_SETTING_H

#include <stdint.h>

typedef enum clockSettingItem {
    CLOCKSETTING_HOUR,
    CLOCKSETTING_MINUTE,
    CLOCKSETTING_SECOND,
    CLOCKSETTING_CENTURY,
    CLOCKSETTING_YAER,
    CLOCKSETTING_MONTH,
    CLOCKSETTING_DAY,
    CLOCKSETTING_LED_MODE,
    CLOCKSETTING_LED_MODESUB,
    CLOCKSETTING_ANTIPOSION_CYCLE,
    CLOCKSETTING_ANTIPOSION_INTERVAL,
    CLOCKSETTING_NUMOFTYPES
} clockSettingItem_t;

uint16_t readStoredSettingValue(clockSettingItem_t item);
uint16_t readSettingValue(clockSettingItem_t item);
void updateSettingValue(clockSettingItem_t item, int8_t update);
void saveCurrentSettingIfNeeded(clockSettingItem_t item);

#endif //IN18_NIXIECLOCK_NIXIE_SETTING_H

#include <sched.h>//
// Created by codetector on 5/2/18.
//
#include <stdio.h>
#include <string.h>
#include <stm32f103xb.h>
#include <clockState.h>
#include "th06c.h"
#include "nixieclock.h"
#include "cmsis_os.h"
#include "led_driver.h"
#include "hv57708.h"
#include "ds3231.h"
#include "sram.h"

#define LONG_BTN_PRESS 500

QueueHandle_t renderQueueHandle;
QueueHandle_t buttonPressHandle;

uint64_t translateDisplay(char *charsToDisplay);

int pinOffsetFromChar(char c);

int8_t buttonStateCompute(uint8_t oldState, uint8_t newState, uint8_t bit);

void reloadSettings();

static char textDisplay[] = {'0', '0', '0', '0', '0', '0', '\0'};


// State Machine Keeping Stuff
int8_t masterClockState = CLOCKSTATE_TIME;
uint32_t lastActivity = 0;
uint32_t lastAntiPosion = 0, antiPosionInterval, antiPoisonTargetTime;
uint8_t antiPosionPosition;
clockSettingItem_t current_setting_item = CLOCKSETTING_HOUR;
uint16_t current_setting_value = 0;
int8_t current_setting_changed = 0;
int8_t statusUpdated = 1;

uint8_t readButtonStates() {
#ifdef IN14
    return ~(uint8_t) (((GPIOA->IDR) >> 4) & 0b111);
#else
    return (uint8_t) (((GPIOA -> IDR) >> 4) & 0b111);
#endif
}

void miscLoop(const void __unused *p) {
//    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 1234);
    uint16_t magic = BKP_REG_READ(0);
    osDelay(10);
    uint8_t buttonStates = readButtonStates();
    uint32_t lastButtonActionTime = 0;
    int8_t lastButtonId = -1;
    reloadSettings();
    for (;;) {
        // Process Buttons
        uint8_t newButtonStates = readButtonStates();
        uint32_t currentTick = HAL_GetTick();
        uint32_t timeSinceLastAction = currentTick - lastButtonActionTime;
        if (newButtonStates != buttonStates) {
            for (uint8_t i = 0; i < 3; i++) {
                int8_t btnChange = buttonStateCompute(buttonStates, newButtonStates, i);
                if (btnChange > 0) { // Is Press
                    lastButtonId = i;
                    lastButtonActionTime = HAL_GetTick();
                    timeSinceLastAction = 0;
                } else if (btnChange < 0) {
                    if ((lastButtonId != -1) && ((timeSinceLastAction) < LONG_BTN_PRESS)) { // Short Press?
                        buttonAction_t buttonAction1 = {.buttonId = lastButtonId, .actionType = 0};
                        xQueueSend(buttonPressHandle, &buttonAction1, 0);
                        lastButtonId = -1;
                    }
                }
            }
        }
        if ((lastButtonId != -1) && (timeSinceLastAction >= LONG_BTN_PRESS)) {
            buttonAction_t buttonAction = {.buttonId = lastButtonId, .actionType = 1};
            xQueueSend(buttonPressHandle, &buttonAction, 0);
            lastButtonId = -1;
        }
        buttonStates = newButtonStates;
        osDelay(10);
    }
}

void reloadSettings() {
    // Antiposion
//    antiPosionInterval = 10000;
    antiPosionInterval = ((uint32_t) readStoredSettingValue(CLOCKSETTING_ANTIPOSION_INTERVAL) * 60000);
    antiPoisonTargetTime = 10 * 1000;
}

int8_t buttonStateCompute(uint8_t oldState, uint8_t newState, uint8_t bit) {
    int8_t returnValue = 0;
    if (((oldState >> bit) & 1U) != ((newState >> bit) & 1U)) {
        returnValue = 1;
        if (((newState >> bit) & 1U)) {
            returnValue = -returnValue;
        }
    }
    return returnValue;
}

void clockRenderLoop(const void __unused *p) {
    initHV57708();
    char *renderTarget = textDisplay;
    uint64_t renderData = 1, renderDataMod = 0;
    //Render Group Generation
    uint64_t g1 = 0b1111111111111100000000001111111111000000000011111111110000000000;
    uint64_t g2 = 0b1111000000000011111111110000000000111111111100000000001111111111;

    uint8_t delay = 8;
    for (;;) {
        if (masterClockState != CLOCKSTATE_OFF) {
            if (xQueueReceive(renderQueueHandle, &renderTarget, delay)) {
                renderDataMod = translateDisplay(textDisplay);
                renderData = renderDataMod;
            }
            osDelay(delay);
            HV_writeData(renderData & g1);
            osDelay(delay);
            HV_writeData(renderData & g2);
        } else {
            osDelay(50);
//            writeAllLED(0x010101);
        }
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"

void buttonHandle() {
    buttonAction_t btnPress;
    if (xQueueReceive(buttonPressHandle, &btnPress, 1)) {
        lastActivity = HAL_GetTick();
        switch (masterClockState) {
            case CLOCKSTATE_TIME:
                if (btnPress.actionType == 1) {
                    switch (btnPress.buttonId) {
                        case 0:
                            masterClockState = CLOCKSTATE_OFF;
                            statusUpdated = 1;
                            break;
                        case 1:
                            masterClockState = CLOCKSTATE_SETTING;
                            current_setting_item = CLOCKSETTING_HOUR;
                            current_setting_value = readStoredSettingValue(current_setting_item);
                            current_setting_changed = 0;
                            statusUpdated = 1;
                            break;
                        case 2:
                            masterClockState = CLOCKSTATE_TUBETEST;
                            break;
                    }
                }
                break;
            case CLOCKSTATE_SETTING:
                if (btnPress.actionType == 1 && btnPress.buttonId == 1) {
                    saveCurrentSettingIfNeeded(current_setting_item);
                    masterClockState = CLOCKSTATE_TIME;
                    reloadSettings();
                }
                if (btnPress.actionType == 0) {
                    switch (btnPress.buttonId) {
                        case 1: // Next Item In Settings
                            saveCurrentSettingIfNeeded(current_setting_item);
                            current_setting_item++;
                            if (current_setting_item >= CLOCKSETTING_NUMOFTYPES) {
                                saveCurrentSettingIfNeeded(current_setting_item);
                                masterClockState = CLOCKSTATE_TIME;
                                reloadSettings();
                            }
                            current_setting_value = readStoredSettingValue(current_setting_item);
                            current_setting_changed = 0;
                            break;
                        case 0:
#ifdef IN14
                            updateSettingValue(current_setting_item, 1);
#else
                            updateSettingValue(current_setting_item, -1);
#endif
                            current_setting_changed = 1;
                            break;
                        case 2:
#ifdef IN14
                            updateSettingValue(current_setting_item, -1);
#else
                            updateSettingValue(current_setting_item, 1);

#endif                            current_setting_changed = 1;
                            break;
                    }
                    current_setting_value = readSettingValue(current_setting_item);
                }
                break;
            case CLOCKSTATE_OFF:
                if (btnPress.actionType == 1) {
                    masterClockState = CLOCKSTATE_TIME;
                }
                break;
            case CLOCKSTATE_TUBETEST:
                if (btnPress.actionType == 1) {
                    if (btnPress.buttonId != 1) {
                        masterClockState = CLOCKSTATE_TUBERECOVER;
                    } else {
                        masterClockState = CLOCKSTATE_TIME;
                    }
                } else {
                    if (btnPress.buttonId == 0) {
                        antiPosionPosition--;
                    } else if (btnPress.buttonId == 2) {
                        antiPosionPosition++;
                    } else {
                        antiPosionPosition = 0;
                    }
                    if (antiPosionPosition > 9) {
                        antiPosionPosition = 0;
                    }
                }
                break;
            case CLOCKSTATE_TUBERECOVER:
                if (btnPress.actionType == 1) {
                    masterClockState = CLOCKSTATE_TIME;
                } else {
                    masterClockState = CLOCKSTATE_TUBETEST;
                }
                break;
        }
    }
}

#pragma clang diagnostic pop

void clockDisplayMechanism() {
    rtcTime_t time;
    time = RTC_readTime();
    sprintf(textDisplay, "%02d%02d%02d", time.hour, time.minute, time.second);
    xQueueSend(renderQueueHandle, &textDisplay, 0);
}

void settingRenderMechanism() {
    if (current_setting_item > 99) {
        current_setting_item = CLOCKSETTING_HOUR;
    }
    sprintf(textDisplay, "%02d%04d", current_setting_item, current_setting_value);
    xQueueSend(renderQueueHandle, &textDisplay, 10);
}

void tubeRecoverMechanism() {
    sprintf(textDisplay, "%06d", ((uint16_t) antiPosionPosition) * 111111);
    antiPosionPosition++;
    if (antiPosionPosition > 9) {
        antiPosionPosition = 0;
    }
    xQueueSend(renderQueueHandle, &textDisplay, 0);
    osDelay(100);
}

void antiPosionMechanism() {
    tubeRecoverMechanism();
    uint32_t millis = HAL_GetTick();
    if ((millis - lastAntiPosion) >= antiPoisonTargetTime) {
        masterClockState = CLOCKSTATE_TIME;
        lastAntiPosion = millis;
    }
}

void antiPosionHandle() {
    if (antiPosionInterval > 0) {
        uint32_t currentTime = HAL_GetTick();
        if (currentTime - lastAntiPosion > antiPosionInterval) {
            masterClockState = CLOCKSTATE_ANTIPOSION;
            lastAntiPosion = currentTime;
        }
    }
}

void tubeTestMechanism() {
    sprintf(textDisplay, "%06d", ((uint16_t) antiPosionPosition) * 111111);
}

void ledHandle() {
    if (masterClockState == CLOCKSTATE_OFF || masterClockState == CLOCKSTATE_SLEEP) {
        if (statusUpdated) {
            writeAllLED(0x0);
        }
    } else {
    }
}

void powerHandle() {
    if (masterClockState == CLOCKSTATE_OFF || masterClockState == CLOCKSTATE_SLEEP) {
        GPIOA->BSRR = 1U << 7;
    } else {
        GPIOA->BRR = (1U << 7);
    }
}

void temperatureMechanism() {
    uint16_t temp = (uint16_t) (getTemperatureC() * 10);
    sprintf(textDisplay, "%02d---%01d", temp / 10, (temp % 10));
    xQueueSend(renderQueueHandle, &textDisplay, 1);
}

void clockMechanismLoop(const void __unused *p) {
    osDelay(100);
    for (;;) {
        buttonHandle(); // Handle Button Presses
        powerHandle();
        ledHandle();

        switch (masterClockState) {
            case CLOCKSTATE_TIME:
                clockDisplayMechanism();
                antiPosionHandle();
                break;
            case CLOCKSTATE_TEMP:
                temperatureMechanism();
                break;
            case CLOCKSTATE_SETTING:
                settingRenderMechanism();
                break;
            case CLOCKSTATE_ANTIPOSION:
                antiPosionMechanism();
                break;
            case CLOCKSTATE_TUBETEST:
                tubeTestMechanism();
                break;
            case CLOCKSTATE_TUBERECOVER:
                tubeRecoverMechanism();
                break;
            default:
                break;
        }
        osDelay(1);
        statusUpdated = 0;
    }
}

/// Other Tasks

uint64_t translateDisplay(char *charsToDisplay) {
    uint64_t bitsRegister = 0;
    uint64_t unit1 = 1;
    for (int i = 0; i < 6; i++) {
        int shift = (uint8_t) (5 - i);
        int offset = pinOffsetFromChar(charsToDisplay[i]);
        if (offset >= 0) {
            bitsRegister |= (unit1 << (offset + (shift * 10)));
        }
    }
    return bitsRegister;
}

int pinOffsetFromChar(char c) {
#ifdef IN14
    int offset = c - '0';
    if (offset < 10) {
        return (10 - offset) % 10;
    }
    return -1;
#else
    int rtnValue = (c - '1');
    if (rtnValue < 0) {
        rtnValue = 9;
    }
    if (rtnValue >=0 && rtnValue < 10) {
        return rtnValue;
    }
    return -1;
#endif
}
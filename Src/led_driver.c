//
// Created by codetector on 5/2/18.
//
#include <stm32f103xb.h>
#include <cmsis_os.h>
#include "led_driver.h"
#include "pins.h"
#include "macros.h"

void ledReset() {
    LED_CONTROL_GPIO -> BRR = (1U << LED_CONTROL_PIN);
    HAL_Delay(10);
}

void ledWrite(uint32_t color) {
    for (int i = 23; i >= 0; i--) {
        if ((color >> i) & 1U) {
            LED_CONTROL_GPIO -> BSRR = (1U << LED_CONTROL_PIN);
            DELAY_50_NOP;
            LED_CONTROL_GPIO -> BRR = 1U << LED_CONTROL_PIN;
            DELAY_0_NOP;
        } else {
            LED_CONTROL_GPIO->BSRR = (1U << LED_CONTROL_PIN);
            DELAY_5_NOP;
            LED_CONTROL_GPIO->BRR = (1U << LED_CONTROL_PIN);
            DELAY_50_NOP;
        }
    }
}

void writeAllLED(uint32_t color) {
    ledReset();
    for (int i = 0; i < LED_COUNT; i++) {
        ledWrite(color);
    }
}

void writeLedWithColor(uint32_t colors[], uint8_t count) {
    int fillTimes = LED_COUNT / count;
    if ((LED_COUNT % count) != 0) fillTimes++;
    ledReset();
    for (int i = 0; i < fillTimes; ++i) {
        for (int j = 0; j < count; ++j) {
            ledWrite(colors[j]);
        }
    }
}
//
// Created by codetector on 5/1/18.
//

#ifndef IN18_NIXIECLOCK_LED_DRIVER_H
#define IN18_NIXIECLOCK_LED_DRIVER_H

#include "stm32f1xx_hal.h"
#include "pins.h"
#include "macros.h"

#define LED_WRITE_HIGH do {    LED_CONTROL_GPIO -> BSRR = (1U << LED_CONTROL_PIN); DELAY_50_NOP; LED_CONTROL_GPIO -> BRR = 1U << LED_CONTROL_PIN; } while(0)
#define LED_WRITE_LOW do {     LED_CONTROL_GPIO -> ODR |= (1U << LED_CONTROL_PIN); DELAY_5_NOP; LED_CONTROL_GPIO -> ODR &= ~(1U << LED_CONTROL_PIN); DELAY_50_NOP; } while(0)
void ledReset();
void ledWrite(uint32_t color);
void writeAllLED(uint32_t color);
void writeLedWithColor(uint32_t *colors, uint8_t count);


#endif //IN18_NIXIECLOCK_LED_DRIVER_H

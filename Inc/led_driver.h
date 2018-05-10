//
// Created by codetector on 5/1/18.
//

#ifndef IN18_NIXIECLOCK_LED_DRIVER_H
#define IN18_NIXIECLOCK_LED_DRIVER_H

#include "stm32f1xx_hal.h"
#include "pins.h"
#include "macros.h"

void ledReset();
void ledWrite(uint32_t color);
void writeAllLED(uint32_t color);
void writeLedWithColor(uint32_t *colors, uint8_t count);


#endif //IN18_NIXIECLOCK_LED_DRIVER_H

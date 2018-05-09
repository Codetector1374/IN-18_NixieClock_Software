//
// Created by codetector on 5/8/18.
//
#include "stm32f1xx_hal.h"
#include "sram.h"

void BKP_REG_WRITE(uint16_t data, uint8_t offset) {
    HAL_RTCEx_BKUPWrite(NULL, 1 + offset, data);
}

uint16_t BKP_REG_READ(uint8_t offset) {
    return (uint16_t) HAL_RTCEx_BKUPRead(NULL, 1 + offset);
}

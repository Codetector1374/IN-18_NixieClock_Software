//
// Created by codetector on 5/8/18.
//
#include "stm32f1xx_hal.h"
#include "sram.h"

void BKP_REG_WRITE(uint16_t data, uint8_t offset) {
    HAL_RTCEx_BKUPWrite(NULL, 1 + offset, data);
}

void BKP_REG_WRITE_BITS(uint16_t data, uint8_t regOffset, uint8_t dataLength, uint8_t dataOffset) {
    assert_param((dataOffset + dataLength) <= 16);
    uint16_t currentRegisterValue = BKP_REG_READ(regOffset);
    uint16_t mask = (uint16_t) ((((uint32_t)1U) << (dataLength + 1)) - 1U);
    currentRegisterValue &= (~mask) << dataOffset;
    currentRegisterValue |= (data & mask) << dataOffset;
    BKP_REG_WRITE(currentRegisterValue, regOffset);
}

uint16_t BKP_REG_READ(uint8_t offset) {
    return (uint16_t) HAL_RTCEx_BKUPRead(NULL, 1 + offset);
}

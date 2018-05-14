//
// Created by Codetector on 2018/5/10.
//

#include "th06c.h"
#include "nixieclock.h"
#include "stm32f1xx_hal.h"

float getTemperatureC() {
    uint8_t command = 0xE3;
    HAL_I2C_Master_Transmit(&hi2c1, TH06C_DEVICE_ADDR, &command, 1, 50);
    osDelay(10);
    uint8_t data[2];
    HAL_I2C_Master_Receive(&hi2c1, TH06C_DEVICE_ADDR, data, 2, 500);
    uint16_t tempRawCode = ((uint16_t)data[0] << 8) | data[1];
    float temperature = (float) (((175.72 * tempRawCode) / 65536.0) - 46.85);
    return temperature;
}

uint8_t getHumidity() {
    uint8_t command = 0xE5;
    HAL_I2C_Master_Transmit(&hi2c1, TH06C_DEVICE_ADDR, &command, 1, 50);
    osDelay(10);
    uint8_t data[2];
    HAL_I2C_Master_Receive(&hi2c1, TH06C_DEVICE_ADDR, data, 2, 500);
    uint16_t tempRawCode = ((uint16_t)data[0] << 8) | data[1];
    uint humidity = (uint8_t) (((125 * tempRawCode) / 65536.0) - 6.0);
    return humidity;
}
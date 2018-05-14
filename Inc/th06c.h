//
// Created by Codetector on 2018/5/10.
//

#ifndef IN18_NIXIECLOCK_TH06C_H
#define IN18_NIXIECLOCK_TH06C_H

#include <stdint.h>

float getTemperatureC();
uint8_t getHumidity();

#define TH06C_DEVICE_ADDR 0b10000000

#endif //IN18_NIXIECLOCK_TH06C_H

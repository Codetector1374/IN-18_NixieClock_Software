//
// Created by codetector on 5/2/18.
//

#ifndef IN18_NIXIECLOCK_HV57708_H
#define IN18_NIXIECLOCK_HV57708_H

#include "stm32f1xx_hal.h"

typedef u_char byte;

void initHV57708();
void HV_writeData(uint64_t bits);

#endif //IN18_NIXIECLOCK_HV57708_H

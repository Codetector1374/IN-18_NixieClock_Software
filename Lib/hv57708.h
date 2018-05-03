//
// Created by codetector on 5/2/18.
//

#ifndef IN18_NIXIECLOCK_HV57708_H
#define IN18_NIXIECLOCK_HV57708_H

#include <FreeRTOS.h>
#include <queue.h>
#include "stm32f1xx_hal.h"

typedef u_char byte;

extern QueueHandle_t renderQueueHandle;

void initHV57708();
void HV_writeData(uint64_t bits);

#endif //IN18_NIXIECLOCK_HV57708_H

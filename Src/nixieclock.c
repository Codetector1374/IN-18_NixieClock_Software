//
// Created by codetector on 5/2/18.
//
#include "nixieclock.h"
#include "cmsis_os.h"
#include "../Lib/led_driver.h"
#include "../Lib/hv57708.h"

QueueHandle_t renderQueueHandle;

uint64_t translateDisplay(char* charsToDisplay) ;
int pinOffsetFromChar(char c) ;
static char renderText[] = {'1','2','3','4','5','6'};

void miscLoop(const void *p) {
    for(;;) {
        char* rFirstLoc = renderText;
        xQueueSend(renderQueueHandle, &rFirstLoc, 100);
        vTaskDelay(1);
    }
}

void clockRenderLoop(const void *p) {
    initHV57708();
    char* renderTarget;
    uint64_t renderData;
    for (;;) {
        xQueueReceive(renderQueueHandle, &renderTarget, 100);
        renderData = translateDisplay(renderTarget);
        HV_writeData(0);
    }
}

uint64_t translateDisplay(char* charsToDisplay) {
    uint64_t bitsRegister = 0;
    uint64_t unit1 = 1;
    for (int i = 0; i < 6; i++) {
        uint8_t shift = (uint8_t) (5 - i);
        bitsRegister |= (unit1 << ((pinOffsetFromChar(charsToDisplay[i]) + (shift * 10))));
    }
    return bitsRegister;
}

int pinOffsetFromChar(char c) {
    int rtnValue = (c - '1');
    if (rtnValue < 0) {
        rtnValue = 9;
    }
    if (rtnValue >=0 && rtnValue < 10) {
        return rtnValue;
    }
    return -1;
}
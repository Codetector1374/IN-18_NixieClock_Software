//
// Created by codetector on 5/2/18.
//
#include "nixieclock.h"
#include "cmsis_os.h"
#include "../Lib/led_driver.h"

void miscLoop(const void *p) {
    for(;;) {
//        writeLedWithColor(colors, 2);
        vTaskDelay(100);
    }
}

void clockRenderLoop(const void *p) {

}
//
// Created by codetector on 5/2/18.
//
#include "hv57708.h"
#include "cmsis_os.h"
#include "pins.h"
#include "macros.h"

#define HV_BL_L HV_GPIO -> BRR = 1U << HV_BL
#define HV_POL_L HV_GPIO -> BRR = 1U << HV_POL
#define HV_LE_L HV_GPIO -> BRR = 1U << HV_LE
#define HV_CLK_L HV_GPIO -> BRR = 1U << HV_CLK
#define HV_D1_L HV_GPIO -> BRR = 1U << HV_D1
#define HV_D2_L HV_GPIO -> BRR = 1U << HV_D2
#define HV_D3_L HV_GPIO -> BRR = 1U << HV_D3
#define HV_D4_L HV_GPIO -> BRR = 1U << HV_D4

#define HV_BL_H HV_GPIO -> BSRR = 1U << HV_BL
#define HV_POL_H HV_GPIO -> BSRR = 1U << HV_POL
#define HV_LE_H HV_GPIO -> BSRR = 1U << HV_LE
#define HV_CLK_H HV_GPIO -> BSRR = 1U << HV_CLK
#define HV_D1_H HV_GPIO -> BSRR = 1U << HV_D1
#define HV_D2_H HV_GPIO -> BSRR = 1U << HV_D2
#define HV_D3_H HV_GPIO -> BSRR = 1U << HV_D3
#define HV_D4_H HV_GPIO -> BSRR = 1U << HV_D4

void initHV57708() {
    HV_POL_H;

    HV_D1_L;
    HV_D2_L;
    HV_D3_L;
    HV_D4_L;

    HV_CLK_L;
    HV_LE_L;
    vTaskDelay(1);
}

void shiftBit(byte bits) {
    HV_CLK_L;

    HV_GPIO -> BSRR = bits << 12;
    DELAY_1_NOP;
    HV_CLK_H;
    DELAY_1_NOP;
    HV_CLK_L;
}

void latch() {
    HV_LE_H;
    DELAY_1_NOP;
    HV_LE_L;
}

void HV_writeData(uint64_t bits) {
    for (uint8_t i = 60; i >= 0 ; i -= 4) {
        shiftBit((byte) ((bits >> i) & 0x1111));
    }
    latch();
}
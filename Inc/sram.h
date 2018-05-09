//
// Created by codetector on 5/8/18.
//

#ifndef IN18_NIXIECLOCK_SRAM_H
#define IN18_NIXIECLOCK_SRAM_H

uint16_t BKP_REG_READ(uint8_t offset);

void BKP_REG_WRITE(uint16_t data, uint8_t offset);

#endif //IN18_NIXIECLOCK_SRAM_H

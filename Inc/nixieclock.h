//
// Created by codetector on 5/2/18.
//

#ifndef IN18_NIXIECLOCK_NIXIECLOCK_HPP
#define IN18_NIXIECLOCK_NIXIECLOCK_HPP

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "nixie_setting.h"

#define IN14

extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;

extern QueueHandle_t renderQueueHandle;
extern QueueHandle_t buttonPressHandle;

extern clockSettingItem_t current_setting_item;
extern uint16_t current_setting_value;
extern int8_t current_setting_changed;
//extern static char textDisplay[];

void miscLoop(void const __unused *p);
void clockRenderLoop(void const __unused *P);
void clockMechanismLoop(const void __unused *p);

#endif //IN18_NIXIECLOCK_NIXIECLOCK_HPP

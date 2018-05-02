#ifndef NIXIE_PINS_H
#define NIXIE_PINS_H

#include "stm32f1xx_hal.h"

#define BUZZER_GPIO GPIOB
//
//
#define BUZZER_PIN GPIO_PIN_5

#define LED_CONTROL_GPIO GPIOA
#define LED_CONTROL_PIN 8
#define LED_COUNT 12

#define HV_GPIO GPIOB
#define HV_BL 8
#define HV_POL 9
#define HV_LE 10
#define HV_CLK 11
#define HV_D1 12
#define HV_D2 13
#define HV_D3 14
#define HV_D4 15

#endif
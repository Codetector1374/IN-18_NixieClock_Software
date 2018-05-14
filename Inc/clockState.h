//
// Created by codetector on 5/4/18.
//

#ifndef IN18_NIXIECLOCK_CLOCKSTATE_H
#define IN18_NIXIECLOCK_CLOCKSTATE_H

#include <stdint.h>


enum clockState {
    CLOCKSTATE_TIME,
    CLOCKSTATE_TEMP,
    CLOCKSTATE_ANTIPOSION,
    CLOCKSTATE_OFF,
    CLOCKSTATE_SLEEP,
    CLOCKSTATE_PEEK,
    CLOCKSTATE_TUBETEST,
    CLOCKSTATE_TUBERECOVER,
    CLOCKSTATE_SETTING
};


typedef struct buttonAction {
    int8_t buttonId;
    int8_t actionType;
} buttonAction_t;

#endif //IN18_NIXIECLOCK_CLOCKSTATE_H

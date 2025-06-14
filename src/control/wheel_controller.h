//
// Created by yasuoki on 24/12/23.
//

#ifndef INC_AERGIA_WHEEL_CONTROLLER_H
#define INC_AERGIA_WHEEL_CONTROLLER_H

#include <Arduino.h>
#include "config.h"
class WheelController {
private:
    enum State {
        STOP,
        TRIGGER,
        FORWARD,
        BACKWARD,
    };
    enum Fase {
        FASE_NONE,
        FASE_A,
        FASE_B
    };
    static State state;
    static uint32_t lastATime;
    static uint32_t lastBTime;
    static Fase lastFase;
    static int8_t wheelAVal;
    static int8_t wheelBVal;
    static Fase triggerFase;
    static int8_t triggerValue;

public:
    static void init();
    static void scanTrigger(int8_t a, int8_t b);
    static int8_t scanFirst(int8_t a, int8_t b);
    static int8_t scanForward(int8_t a, int8_t b);
    static int8_t scanBackward(int8_t a, int8_t b);
    static int8_t scanWheel(int8_t a, int8_t b);
    static int8_t scan(uint32_t now);
};

#endif //INC_AERGIA_WHEEL_CONTROLLER_H

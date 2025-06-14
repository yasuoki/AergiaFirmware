//
// Created by yasuoki on 24/12/23.
//

#ifndef INC_AERGIA_JOYSTICK_CONTROLLER_H
#define INC_AERGIA_JOYSTICK_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

#if defined(USE_JOYSTICK_CONTROLLER)

class AS5600;

class JoystickController {
private:
    static uint32_t lastTime;
    static AS5600 *axisSensors[];
    static float axisAngles[3];
    static uint8_t lastCode;
public:
    static void init();
    static float readAngle(int axis);
    static uint8_t scan(uint32_t now);
};

#endif
#endif //INC_AERGIA_JOYSTICK_CONTROLLER_H

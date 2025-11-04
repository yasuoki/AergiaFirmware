//
// Created by yasuoki on 24/12/23.
//

#ifndef INC_AERGIA_WHEEL_CONTROLLER_H
#define INC_AERGIA_WHEEL_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class WheelController {
private:
    static PinStatus sensor_A_val;
    static PinStatus sensor_B_val;

public:
    static void init();
    static int8_t scan(uint32_t now);
};

#endif //INC_AERGIA_WHEEL_CONTROLLER_H

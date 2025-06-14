//
// Created by Yasuoki on 2025/04/10.
//
#ifndef INC_AERGIA_RANGING_CONTROLLER_H
#define INC_AERGIA_RANGING_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class Adafruit_VL6180X;

class RangingController {
private:
	static Adafruit_VL6180X vl6180X;
	static uint8_t lastRange;
	static uint32_t last;
public:
	static void init();
	static uint8_t scan(uint32_t now);
};

#endif //INC_AERGIA_RANGING_CONTROLLER_H

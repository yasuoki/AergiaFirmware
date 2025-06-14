//
// Created by Yasuoki on 2025/04/10.
//
#include "ranging_controller.h"

#include "Adafruit_VL6180X.h"

Adafruit_VL6180X RangingController::vl6180X;
uint8_t RangingController::lastRange = -1;
uint32_t RangingController::last = 0;

void RangingController::init()
{
	vl6180X.begin();
}

uint8_t RangingController::scan(uint32_t now)
{
	if(now - last > 200000UL) {
		uint8_t range = vl6180X.readRange();
		uint8_t status = vl6180X.readRangeStatus();

		if (status == VL6180X_ERROR_NONE) {
			lastRange = range;
		} else {
			lastRange = 255;
		}
		last = now;
	}
	return lastRange;
}


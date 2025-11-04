//
// Created by yasuoki on 24/12/23.
//

#include "wheel_controller.h"

PinStatus WheelController::sensor_A_val = PinStatus::LOW;
PinStatus WheelController::sensor_B_val = PinStatus::LOW;

void WheelController::init()
{
    pinMode(PIN_WHEEL_A, INPUT);
    pinMode(PIN_WHEEL_B, INPUT);
	sensor_A_val = digitalRead(PIN_WHEEL_A);
	sensor_B_val = digitalRead(PIN_WHEEL_B);
}

int8_t WheelController::scan(uint32_t now)
{
    PinStatus a = digitalRead(PIN_WHEEL_A);
	PinStatus b = digitalRead(PIN_WHEEL_B);

	int8_t direction = 0;
	if(a != sensor_A_val) {
		if (a == PinStatus::HIGH) {
			if (sensor_B_val == PinStatus::HIGH) {
				// right
				direction = 1;
			} else {
				// left
				direction = -1;
			}
		} else {
			if (sensor_B_val == PinStatus::HIGH) {
				// left
				direction = -1;
			} else {
				// right
				direction = 1;
			}
		}
	} else if(b != sensor_B_val) {
		if(b == PinStatus::HIGH) {
			if(a == PinStatus::HIGH) {
				// left
				direction = -1;
			} else {
				// right
				direction = 1;
			}
		} else {
			if (a == PinStatus::HIGH) {
				// right
				direction = 1;
			} else {
				// left
				direction = -1;
			}
		}
	}
	sensor_A_val = a;
	sensor_B_val = b;
	return direction;
}

//
// Created by yasuoki on 24/12/23.
//

#include "joystick_controller.h"

#if defined(USE_JOYSTICK_CONTROLLER)

#include "i2c_multiplexer.h"
#include <AS5600.h>

AS5600 axis0;
AS5600 axis1;
AS5600 axis2;

uint32_t JoystickController::lastTime = 0;
AS5600 *JoystickController::axisSensors[] = {
    &axis0, &axis1, &axis2
};
float JoystickController::axisAngles[3] = {0, 0, 0};
uint8_t JoystickController::lastCode = 0;

void JoystickController::init() {
    for(int n = 0; n < 3; n++) {
        I2CMultiplexer::select(n);
        AS5600 *as5600 = axisSensors[n];
        as5600->begin();
        as5600->setDirection(AS5600_CLOCK_WISE);
        float pos = as5600->readAngle() * AS5600_RAW_TO_DEGREES;
        as5600->setOffset(-pos);
    }
}

float JoystickController::readAngle(int axis)
{
	return axisAngles[axis];
}

uint8_t JoystickController::scan(uint32_t now)
{
    if(now - lastTime > 20) {
        uint8_t code = 0;
		for(int n = 0; n < 3; n++) {
			I2CMultiplexer::select(n);
			AS5600 *as5600 = axisSensors[n];
			float a = as5600->readAngle() * AS5600_RAW_TO_DEGREES;
			if(a > 180.0f)
				a -= 360.0f;
			if( fabs(a) > 1) {
				code |= (1 << n);
				axisAngles[n] = a;
			} else {
				axisAngles[n] = 0;
			}
		}
        lastCode = code;
        lastTime = now;
    }
    return lastCode;
}

#endif
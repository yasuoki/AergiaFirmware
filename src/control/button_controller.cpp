//
// Created by Yasuoki on 2025/05/15.
//

#include "button_controller.h"

#include "neopixel_controller.h"
uint8_t ButtonController::_lastState;
uint32_t ButtonController::_lastTime;

void ButtonController::init()
{
	_lastState = 0;
	_lastTime = 0;
#if defined(PIN_BUTTON_0)
	pinMode(PIN_BUTTON_0, INPUT_PULLUP);
#endif
#if defined(PIN_BUTTON_1)
	pinMode(PIN_BUTTON_1, INPUT_PULLUP);
#endif
}

uint8_t ButtonController::scan(uint32_t now)
{
	if(now - _lastTime > 10000L) {
		uint8_t code = 0;
#if defined(PIN_BUTTON_0)
		if (digitalRead(PIN_BUTTON_0) == LOW) {
			code |= BUTTON_STATE_0;
		}
#endif
#if defined(PIN_BUTTON_1)
		if (digitalRead(PIN_BUTTON_1) == LOW) {
			code |= BUTTON_STATE_1;
		}
#endif
		_lastTime = now;
		_lastState = code;
	}
	return _lastState;
}

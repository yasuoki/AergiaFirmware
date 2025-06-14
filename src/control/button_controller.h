//
// Created by Yasuoki on 2025/05/15.
//

#ifndef INC_AERGIA_BUTTON_CONTROLLER_H
#define INC_AERGIA_BUTTON_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class ButtonController final {
private:
	static uint32_t	_lastTime;
	static uint8_t _lastState;
	ButtonController() {}
	~ButtonController() {}
	static int _neoPixelMap[];

public:
	static void init();
	static uint8_t scan(uint32_t now);
	static void clearNeoPixels();
	static void showNeoPixels();
	static void setNeoPixelColor(uint16_t  n, uint32_t color);
};

#endif //INC_AERGIA_BUTTON_CONTROLLER_H

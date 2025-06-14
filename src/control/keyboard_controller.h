//
// Created by yasuoki on 24/12/23.
//

#ifndef INC_AERGIA_KEYBOARD_CONTROLLER_H
#define INC_AERGIA_KEYBOARD_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class Adafruit_SSD1306;

class KeyboardController {
private:
	static int _scanFase;
	static uint32_t _lastCode;
	static uint32_t _lastTime;
	static uint32_t _makeCode;

	static void _scanImpl(uint32_t now);
public:
	static void init();
	static uint32_t scan(uint32_t now);
};


#endif //INC_AERGIA_KEYBOARD_CONTROLLER_H

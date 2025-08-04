//
// Created by Yasuoki on 2025/05/18.
//

#ifndef INC_AERGIA_NEO_PIXEL_CONTROLLER_H
#define INC_AERGIA_NEO_PIXEL_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class Adafruit_NeoPixel;

class NeoPixelController {
	static Adafruit_NeoPixel _neoPixels;

public:
	static void init();
	static void clearNeoPixels();
	static bool showNeoPixels();
	static void setNeoPixelColor(uint16_t  n, uint32_t color);
};

#endif //INC_AERGIA_NEO_PIXEL_CONTROLLER_H

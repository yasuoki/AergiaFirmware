//
// Created by Yasuoki on 2025/05/18.
//

#include "neopixel_controller.h"

#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel NeoPixelController::_neoPixels(NEO_PIXEL_LED_COUNT, PIN_NEO_PIXEL_LED, NEO_GRB + NEO_KHZ800);
bool updateColor = false;
void NeoPixelController::init()
{
	_neoPixels.begin();
	_neoPixels.clear();
	_neoPixels.show();
}
void NeoPixelController::clearNeoPixels()
{
	_neoPixels.clear();
	_neoPixels.show();
	updateColor = false;
}
void NeoPixelController::showNeoPixels()
{
	if(updateColor) {
		_neoPixels.show();
		updateColor = false;
	}
}
void NeoPixelController::setNeoPixelColor(uint16_t  n, uint32_t color)
{
	_neoPixels.setPixelColor(n, color);
	updateColor = true;
}

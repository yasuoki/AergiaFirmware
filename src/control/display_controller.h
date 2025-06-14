//
// Created by yasuoki on 24/12/26.
//

#ifndef INC_AERGIA_DISPLAY_CONTROLLER_H
#define INC_AERGIA_DISPLAY_CONTROLLER_H

#include <Arduino.h>
#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#if defined(USE_SSD1306_DRIVER_ARRAY)
#include <Adafruit_SSD1306.h>
#endif

typedef struct _FrameSize {
	uint16_t 	width;
	uint16_t 	height;
} FrameSize;

class ScreenGFX {
public:
	Adafruit_GFX *	gfx;
	virtual int16_t offsetX() { return 0; }
	virtual int16_t offsetY() { return 0; }
	virtual void clear() {}
	virtual void display() {}
	virtual void screenOff() {}
	virtual void screenOn() {}
};

class ST7789GFX : public ScreenGFX {
private:
	Adafruit_ST7789	_st7789;
public:
	ST7789GFX(SPIClass *spiClass, int8_t cs, int8_t dc,int8_t rst);
	virtual void clear();
	virtual void screenOff();
	virtual void screenOn();
};

#if defined(USE_SSD1306_DRIVER_ARRAY)
class SSD1306GFX : public ScreenGFX {
private:
	Adafruit_SSD1306 _ssd1306;
public:
	SSD1306GFX(uint8_t w, uint8_t h, TwoWire *twi,
			   int8_t rst_pin);
	virtual int16_t offsetX() ;
	virtual int16_t offsetY() ;
	virtual void clear() ;
	virtual void display() ;
};
#endif

class DisplayController {
private:
	static ST7789GFX _mainScreen;

#if defined(USE_SSD1306_DRIVER_ARRAY)
	static SSD1306GFX* _subScreenArray;
#endif
public:
    static void init();
#if defined(USE_SSD1306_DRIVER_ARRAY)
	static void initScreenArray();
#endif
	static uint16_t rgb565Color(uint8_t r, uint16_t g, uint8_t b);
	static ScreenGFX* selectScreen(int16_t n);
	static ScreenGFX* getScreen(int16_t n);
};

#endif //INC_AERGIA_DISPLAY_CONTROLLER_H

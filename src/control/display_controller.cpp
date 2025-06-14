//
// Created by yasuoki on 24/12/26.
//

#include "display_controller.h"
#include <SPI.h>
#if defined(USE_SSD1306_DRIVER_ARRAY)
#include "../i2c_multiplexer.h"
#endif
#include "processor.h"

ST7789GFX::ST7789GFX(arduino::SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst)
		: _st7789(spiClass, cs, dc, rst)
{
	gfx = &_st7789;
}
void ST7789GFX::clear() {
	_st7789.fillScreen(0);
}

void ST7789GFX::screenOff() {
#if defined(PIN_ST7789_BLK) && ( PIN_ST7789_BLK != -1)
	digitalWrite(PIN_ST7789_BLK, LOW);
#endif
	((Adafruit_ST7789*)gfx)->enableDisplay(false);
}

void ST7789GFX::screenOn() {
	((Adafruit_ST7789*)gfx)->enableDisplay(true);
#if defined(PIN_ST7789_BLK) && ( PIN_ST7789_BLK != -1)
	digitalWrite(PIN_ST7789_BLK, HIGH);
#endif
}

#if defined(USE_SSD1306_DRIVER_ARRAY)
SSD1306GFX::SSD1306GFX(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin)
		: _ssd1306(w, h, twi, rst_pin)
{
	gfx = &_ssd1306;
	if(!_ssd1306.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS, false)) {
		Processor::log("SSD1306 Begin Err");
		for(;;); // reboot by wdt
	}
	_ssd1306.clearDisplay();
}

int16_t SSD1306GFX::offsetX() {
	return SSD1306_OFFSET_X;
}
int16_t SSD1306GFX::offsetY() {
	return SSD1306_OFFSET_Y;
}
void SSD1306GFX::clear() {
	_ssd1306.clearDisplay();
}
void SSD1306GFX::display() {
	_ssd1306.display();
}
#endif

ST7789GFX DisplayController::_mainScreen(&SPI, PIN_ST7789_CS, PIN_ST7789_DC, PIN_ST7789_RESET);

#if defined(USE_SSD1306_DRIVER_ARRAY)
alignas(SSD1306GFX) unsigned char _subScreenArrayBuffer[sizeof(SSD1306GFX) * SSD1306_ARRAY_COUNT];
SSD1306GFX* DisplayController::_subScreenArray = (SSD1306GFX*)_subScreenArrayBuffer;

void DisplayController::initScreenArray()
{
	for(int16_t i = 0; i < SSD1306_ARRAY_COUNT; i++) {
		SSD1306GFX *screen = (SSD1306GFX*)selectScreen((int16_t)(i+1));
		if(screen == nullptr)
			Processor::log("screen null");
		if(screen != nullptr) {
			new (screen)SSD1306GFX(SSD1306_WIDTH, SSD1306_HEIGHT, &Wire, PIN_SSD1306_RESET);
			screen->gfx->setTextColor(SSD1306_WHITE);
			screen->gfx->setTextSize(1);
		}
	}
}
#endif

void DisplayController::init()
{
	SPI.setTX(PIN_ST7789_TX);
	SPI.setRX(PIN_ST7789_RX);
	SPI.setCS(PIN_ST7789_CS);
	SPI.setSCK(PIN_ST7789_SCK);
	pinMode(PIN_ST7789_BLK, OUTPUT);
	digitalWrite(PIN_ST7789_BLK,HIGH);
	((Adafruit_ST7789*)_mainScreen.gfx)->init(ST7789_WIDTH,ST7789_HEIGHT);
	_mainScreen.gfx->setRotation(2);
}

ScreenGFX* DisplayController::selectScreen(int16_t n)
{
	if(n == 0)
		return &_mainScreen;
#if defined(USE_SSD1306_DRIVER_ARRAY)
	n--;
	if(n < 0 || SSD1306_ARRAY_COUNT <= n) {
		return nullptr;
	}
	I2CMultiplexer::select(n);
	return &_subScreenArray[n];
#else
	return nullptr;
#endif
}

ScreenGFX* DisplayController::getScreen(int16_t n)
{
	if(n == 0)
		return &_mainScreen;
#if defined(USE_SSD1306_DRIVER_ARRAY)
	n--;
	if(n < 0 || SSD1306_ARRAY_COUNT <= n) {
		return nullptr;
	}
	return &_subScreenArray[n];
#else
	return nullptr;
#endif
}

uint16_t DisplayController::rgb565Color(uint8_t r, uint16_t g, uint8_t b)
{
	// '565' color
	uint16_t rb = ((r>>3)&0x1f);
	uint16_t gb = ((g>>2)&0x3f);
	uint16_t bb = ((b>>3)&0x1f);
	uint16_t color = (rb << 11) | (gb << 5) | bb;
	return color;
}

//
// Created by Yasuoki on 2024/12/29.
//

#ifndef INC_AERGIA_DRIVER_H
#define INC_AERGIA_DRIVER_H

#include <Arduino.h>
#include "config.h"

#define IS_COMBINATION_KEY(c)  ((0x79 <= (c) && (c) <= 0x7d))
#define MODIFIER_BIT(m) (((uint8_t)(1 << ((m) & 0x07))))
#define IS_MODIFIER_KEY(m) ((0xe0 <= (m) && (m) <= 0xe7))
#define IS_MEDIA_KEY(m) ((0xe8 <= (m) && (m) <= 0xfb))

class Adafruit_USBD_HID;

class Driver final {
private:
	static Adafruit_USBD_HID	hid;

	Driver() {}
	~Driver() {}
	static int findReportIndex(uint8_t code);

public:
	static void init();
	static void task();
	static bool isConnected();

	static void mouseMove(int8_t x, int8_t y);
	static void mouseWheel(int8_t w);
	static void mouseButtonPress(uint8_t button);
	static void mouseButtonRelease(uint8_t button);

	static bool isModifierPressed(uint8_t m=0);
	static uint8_t findPressedModifierKey();
	static bool keyPress(uint8_t usageCode);
	static bool keyRelease(uint8_t usageCode);
	static void releaseAllKey();
};

#endif //INC_AERGIA_DRIVER_H

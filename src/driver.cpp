//
// Created by Yasuoki on 2024/12/29.
//
#include "driver.h"
#include <Adafruit_TinyUSB.h>

Adafruit_USBD_HID Driver::hid;

hid_keyboard_report_t keyboardReport;
hid_mouse_report_t mouseReport;

#define RID_KEYBOARD 		   1
#define RID_MOUSE    		   2
#define RID_CONSUMER_CONTROL   3

#ifndef HID_USAGE_CONSUMER_SCAN_PREVIOUS
#  ifdef HID_USAGE_CONSUMER_SCAN_PREVIOUS_TRACK
#    define HID_USAGE_CONSUMER_SCAN_PREVIOUS HID_USAGE_CONSUMER_SCAN_PREVIOUS_TRACK
#  else
#    define HID_USAGE_CONSUMER_SCAN_PREVIOUS 0x00B6
#  endif
#endif

#ifndef HID_USAGE_CONSUMER_SCAN_NEXT
#  ifdef HID_USAGE_CONSUMER_SCAN_NEXT_TRACK
#    define HID_USAGE_CONSUMER_SCAN_NEXT HID_USAGE_CONSUMER_SCAN_NEXT_TRACK
#  else
#    define HID_USAGE_CONSUMER_SCAN_NEXT 0x00B5
#  endif
#endif

#ifndef HID_USAGE_CONSUMER_AL_LOCAL_BROWSER
#  ifdef HID_USAGE_CONSUMER_AL_LOCAL_MACHINE_BROWSER
#    define HID_USAGE_CONSUMER_AL_LOCAL_BROWSER HID_USAGE_CONSUMER_AL_LOCAL_MACHINE_BROWSER
#  else
#    define HID_USAGE_CONSUMER_AL_LOCAL_BROWSER 0x0194
#  endif
#endif

const uint16_t consumerUsage[] = {
	HID_USAGE_CONSUMER_PLAY_PAUSE,
	0, // stop cd
	HID_USAGE_CONSUMER_SCAN_PREVIOUS,
	HID_USAGE_CONSUMER_SCAN_NEXT,
	0, // eject cd
	HID_USAGE_CONSUMER_VOLUME_INCREMENT,
	HID_USAGE_CONSUMER_VOLUME_DECREMENT,
	HID_USAGE_CONSUMER_MUTE,
	HID_USAGE_CONSUMER_AL_LOCAL_BROWSER,
	HID_USAGE_CONSUMER_AC_BACK,
	HID_USAGE_CONSUMER_AC_FORWARD,
	HID_USAGE_CONSUMER_AC_STOP,
	HID_USAGE_CONSUMER_STOP,
	HID_USAGE_CONSUMER_AC_SEARCH,
	0, // scroll up
	0, // scroll down
	0, // edit
	HID_USAGE_CONSUMER_SLEEP,
	0, // coffee
	HID_USAGE_CONSUMER_AC_REFRESH,
	HID_USAGE_CONSUMER_AL_CALCULATOR,
};

uint8_t const desc_hid_report[] =
{
	TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
	TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(RID_MOUSE)),
	TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))
};

void Driver::init()
{
#ifdef USE_TINYUSB
	TinyUSB_Device_Init(0);
#endif

	TinyUSBDevice.clearConfiguration();
	TinyUSBDevice.setManufacturerDescriptor("Yonabe Factory");
	TinyUSBDevice.setProductDescriptor(DEVICE_MODEL);
	if(!TinyUSBDevice.isInitialized())
		TinyUSBDevice.begin(0);

	memset(&keyboardReport, 0, sizeof(keyboardReport));
	memset(&mouseReport, 0, sizeof(mouseReport));
	hid.setPollInterval(2);
	hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
	hid.begin();

	if (TinyUSBDevice.mounted()) {
		TinyUSBDevice.detach();
		delay(10);
		TinyUSBDevice.attach();
		delay(10);
	}

}

void Driver::task() {
#ifdef USE_TINYUSB
	TinyUSB_Device_Task();
#endif
}


bool Driver::isConnected()
{
	return TinyUSBDevice.mounted();
}

void Driver::mouseMove(int8_t x, int8_t y)
{
	mouseReport.x = x;
	mouseReport.y = y;
	mouseReport.wheel = 0;
	hid.sendReport(RID_MOUSE,&mouseReport,sizeof(mouseReport));
}

void Driver::mouseWheel(int8_t w)
{
	mouseReport.x = 0;
	mouseReport.y = 0;
	mouseReport.wheel = w;
	hid.sendReport(RID_MOUSE,&mouseReport,sizeof(mouseReport));
}

void Driver::mouseButtonPress(uint8_t button)
{
	uint8_t  newState = mouseReport.buttons | button;
	if(newState != mouseReport.buttons) {
		mouseReport.x = 0;
		mouseReport.y = 0;
		mouseReport.wheel = 0;
		mouseReport.buttons = newState;
		hid.sendReport(RID_MOUSE, &mouseReport, sizeof(mouseReport));
	}
}

void Driver::mouseButtonRelease(uint8_t button)
{
	uint8_t  newState = mouseReport.buttons & (~button);
	if(newState != mouseReport.buttons) {
		mouseReport.x = 0;
		mouseReport.y = 0;
		mouseReport.wheel = 0;
		mouseReport.buttons = newState;
		hid.sendReport(RID_MOUSE, &mouseReport, sizeof(mouseReport));
	}
}

int Driver::findReportIndex(uint8_t code)
{
	for(int i=2; i < 6; i++) {
		if(keyboardReport.keycode[i] == code)
			return i;
	}
	return -1;
}

uint8_t Driver::findPressedModifierKey() {
	uint8_t m = 0;
	uint8_t r = keyboardReport.modifier;
	if(r != 0) {
		for(int i = 0; i < 8; i++) {
			uint8_t bit = 1 << i;
			if((r & bit) != 0) {
				m = 0xe0 + i;
				break;
			}
		}
	}
	return m;
}

bool Driver::isModifierPressed(uint8_t code) {
	if(code == 0)
		return keyboardReport.modifier != 0;
	uint8_t bit = MODIFIER_BIT(code);
	return (keyboardReport.modifier & bit) != 0;
}

bool Driver::keyPress(uint8_t code)
{
	if(IS_MEDIA_KEY(code)) {
		uint16_t usage = consumerUsage[code - 0xe8];
		if (usage != 0)
			hid.sendReport16(RID_CONSUMER_CONTROL, usage);
	} else {
		if (IS_MODIFIER_KEY(code)) {
			uint8_t bit = MODIFIER_BIT(code);
			uint8_t newBit = (keyboardReport.modifier | bit);
			if (keyboardReport.modifier == newBit) {
				// toggle
				newBit = (keyboardReport.modifier & (~bit));
			}
			keyboardReport.modifier = newBit;
		} else {
			if (findReportIndex(code) != -1)
				return false;
			int i = findReportIndex(0);
			if (i == -1)
				return false;
			keyboardReport.keycode[i] = code;
		}
		hid.sendReport(RID_KEYBOARD, &keyboardReport, sizeof(keyboardReport));
	}
	return true;
}

bool Driver::keyRelease(uint8_t code)
{
	if(IS_MEDIA_KEY(code)) {
		uint16_t usage = consumerUsage[code - 0xe8];
		if (usage != 0)
			hid.sendReport16(RID_CONSUMER_CONTROL, 0);
	} else {
		if (IS_MODIFIER_KEY(code)) {
			uint8_t bit = MODIFIER_BIT(code);
			uint8_t newBit = (keyboardReport.modifier & (~bit));
			if (keyboardReport.modifier == newBit)
				return false;
			keyboardReport.modifier = newBit;
		} else {
			int i = findReportIndex(code);
			if (i < 0)
				return false;
			keyboardReport.keycode[i] = 0;
		}
		//Serial.printf("keyRelease %x\r\n", code);
		hid.sendReport(RID_KEYBOARD, &keyboardReport, sizeof(keyboardReport));
	}
	return true;
}

void Driver::releaseAllKey() {
	if(keyboardReport.modifier != 0) {
		keyboardReport.modifier = 0;
		hid.sendReport(RID_KEYBOARD, &keyboardReport, sizeof(keyboardReport));
	}
}

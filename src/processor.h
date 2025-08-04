//
// Created by Yasuoki on 2024/12/29.
//

#ifndef INC_AERGIA_PROCESSOR_H
#define INC_AERGIA_PROCESSOR_H

#include <Arduino.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "config.h"

class Processor {
protected:
	static uint32_t _lastUsbConnectionCheck;
	static bool _currentUsbConnect;

	static bool _currentSerialConnect;
	static uint8_t _currentButtonState;
	static uint32_t _lastButtonDownTime[BUTTON_COUNT];
	static uint32_t _lastButtonUpTime[BUTTON_COUNT];

	static uint16_t _currentKeyState;
	static uint32_t _lastKeyDownTime[KEYBOARD_KEY_COUNT];
	static uint32_t _lastKeyUpTime[KEYBOARD_KEY_COUNT];

	static int8_t _currentWheelDelta;
	static int8_t _currentWheelState;
	static uint32_t _lastWheelTime;

#if defined(JOYSTICK_CONTROLLER)
	static int _currentJoystickState;
	static float _currentJoystickX;
	static float _currentJoystickY;
	static float _currentJoystickZ;
	static float _currentJoystickMX;
	static float _currentJoystickMY;
	static float _currentJoystickMZ;
	static bool _currentJoystickButtonState;
#endif
	static uint8_t _currentRangingState;

	static int _currentApplication;
	static int _currentPage;
	static uint32_t _lastActionTime;
	static uint32_t _lastActionElapsedTime;

	Processor();

	static bool onKeyScanTimer(struct repeating_timer *t);
	void startTimrScan();
	void stopTimerScan();

public:
	static void log(const char *text);
    static Processor *createProcessor(ProcessorMode mode);

	virtual bool init();
	static bool isKeySwitchId(uint16_t id);
	static uint16_t KeySwitchIndexFromControlIdTo(uint16_t id);
	static bool isButtonId(uint16_t id);
	static uint16_t ButtonIndexFromControlIdTo(uint16_t id);

	virtual void onKeyDown(ControlId from, uint32_t now);
	virtual void onKeyUp(ControlId from, uint32_t now);
	virtual void onKeyInput(ControlId from, uint32_t now);
	virtual void onKeyLongPress(ControlId from, uint32_t now);
	virtual void onWheelBegin(ControlId from, uint32_t now, int8_t w);
	virtual void onWheel(ControlId from, uint32_t now, int8_t w);
	virtual void onWheelEnd(ControlId from, uint32_t now, int8_t w);
	virtual void onStickBeginMove(ControlId from, uint32_t now);
	virtual void onStickMove(ControlId from, uint32_t now, float x, float y, float z);
	virtual void onStickEndMove(ControlId from, uint32_t now);
	virtual void onStickBeginRotate(ControlId from, uint32_t now);
	virtual void onStickRotate(ControlId from, uint32_t now, float x, float y, float z);
	virtual void onStickEndRotate(ControlId from, uint32_t now);
	virtual void onEnter(ControlId from, uint32_t now);
	virtual void onLeave(ControlId from, uint32_t now);
	virtual void onLoad(ControlId from, uint32_t now, const uint8_t *data, size_t size);
	virtual void onUsbMount(ControlId from, uint32_t now);
	virtual void onUsbUnmount(ControlId from, uint32_t now);
	virtual void onSerialConnect(ControlId from, uint32_t now);
	virtual void onSerialDisconnect(ControlId from, uint32_t now);
	virtual void onSerial(ControlId from, uint32_t now, const uint8_t *data, size_t size);
	virtual void onLoop(uint32_t now, bool eventFired);

	void process(uint32_t now);
};


#endif //INC_AERGIA_PROCESSOR_H

//
// Created by Yasuoki on 2024/12/29.
//
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TinyUSB.h>
#include "visual_manager.h"
#include "visual_renderer.h"
#include "processor.h"
#include "update_processor.h"
#include "km_processor.h"
#include "control/button_controller.h"
#include "control/keyboard_controller.h"
#include "control/wheel_controller.h"
#include "control/joystick_controller.h"
#include "control/ranging_controller.h"
#include "control/display_controller.h"

uint32_t Processor::_lastUsbConnectionCheck = 0;
bool Processor::_currentUsbConnect = false;

bool Processor::_currentSerialConnect = false;

uint8_t Processor::_currentButtonState = 0;
uint32_t Processor::_lastButtonDownTime = 0;
uint32_t Processor::_lastButtonUpTime = 0;

uint16_t Processor::_currentKeyState = 0;
uint32_t Processor::_lastKeyDownTime = 0;
uint32_t Processor::_lastKeyUpTime = 0;

int8_t Processor::_currentWheelDelta = 0;
int8_t Processor::_currentWheelState = 0;
uint32_t Processor::_lastWheelTime = 0;

uint8_t Processor::_currentRangingState = 0;
int Processor::_currentApplication = -1;
int Processor::_currentPage = -1;

uint32_t Processor::_lastActionTime = 0;
uint32_t Processor::_lastActionElapsedTime = 0;

uint8_t recvBuffer[256];

Processor *Processor::createProcessor(ProcessorMode mode)
{
	Processor *p;
    switch (mode) {
        case Configure:
            p = new UpdateProcessor();
			p->init();
			return p;
        case KeyboardMouse:
            p = new KMProcessor();
			if(p->init())
				return p;
			p = new UpdateProcessor();
			p->init();
			return p;
        case MultiAxisJoyStick:
            return nullptr;
    }
    return nullptr;
}


Processor::Processor()
{
	_lastKeyDownTime = 0;
	_lastKeyUpTime = 0;
	_lastButtonDownTime = 0;
	_lastButtonUpTime = 0;
	_lastWheelTime = 0;
	_lastActionTime = millis();
	_lastActionElapsedTime = 0;
}

void Processor:: log(const char *text)
{
	ScreenGFX *screen = DisplayController::selectScreen(0);
	screen->gfx->fillRect(14,106,212,38,0);
	VisualRenderer::setLargeFont(screen);
	VisualRenderer::drawText(screen, 134, text);
}

bool Processor::init()
{
	return true;
}

bool Processor::isKeySwitchId(uint16_t id)
{
	return id >= ControlId::KeySwitch0 && id <= ControlId::KeySwitch15;
}

uint16_t Processor::KeySwitchIndexFromControlIdTo(uint16_t id)
{
	return id - ControlId::KeySwitch0;
}

bool Processor::isButtonId(uint16_t id)
{
	return id >= ControlId::Button0 && id <= ControlId::Button1;
}

uint16_t Processor::ButtonIndexFromControlIdTo(uint16_t id)
{
	return id - ControlId::Button0;
}

void Processor::onKeyDown(ControlId from, uint32_t now) {}
void Processor::onKeyUp(ControlId from, uint32_t now) {}
void Processor::onKeyInput(ControlId from, uint32_t now) {}
void Processor::onKeyLongPress(ControlId from, uint32_t now) {}
void Processor::onWheelBegin(ControlId from, uint32_t now, int8_t w) {}
void Processor::onWheel(ControlId from, uint32_t now, int8_t w) {}
void Processor::onWheelEnd(ControlId from, uint32_t now, int8_t w) {}
void Processor::onStickBeginMove(ControlId from, uint32_t now) {}
void Processor::onStickMove(ControlId from, uint32_t now, float x, float y, float z) {}
void Processor::onStickEndMove(ControlId from, uint32_t now) {}
void Processor::onStickBeginRotate(ControlId from, uint32_t now) {}
void Processor::onStickRotate(ControlId from, uint32_t now, float x, float y, float z) {}
void Processor::onStickEndRotate(ControlId from, uint32_t now) {}
void Processor::onEnter(ControlId from, uint32_t now) {}
void Processor::onLeave(ControlId from, uint32_t now) {}
void Processor::onSerial(ControlId from, uint32_t now, const uint8_t *data, size_t size) {}
void Processor::onLoop(uint32_t now) {}
void Processor::onLoad(ControlId from, uint32_t now, const uint8_t *data, size_t size) {}
void Processor::onUsbMount(ControlId from, uint32_t now) {}
void Processor::onUsbUnmount(ControlId from, uint32_t now) {}
void Processor::onTimer(ControlId from, uint32_t now, const uint8_t *data, size_t size) {}
void Processor::onSerialConnect(ControlId from, uint32_t now) {}
void Processor::onSerialDisconnect(ControlId from, uint32_t now) {}

void Processor::process(uint32_t now) {

	bool eventFired = false;
#ifdef TINYUSB_NEED_POLLING_TASK
	// Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
#endif
	if(now - _lastUsbConnectionCheck > 2000000L) {
		if(!TinyUSBDevice.suspended()) {
			if(!_currentUsbConnect) {
				_currentUsbConnect = true;
				onUsbMount(ControlId::Main, now);
				eventFired = true;
			}
		} else {
			if(_currentUsbConnect) {
				_currentUsbConnect = false;
				onUsbUnmount(ControlId::Main, now);
				eventFired = true;
			}
		}
		_lastUsbConnectionCheck = now;
	}
	if(!_currentUsbConnect)
		return;

	if(Serial) {
		if(!_currentSerialConnect) {
			_currentSerialConnect = true;
			onSerialConnect(ControlId::SerialPort, now);
			eventFired = true;
		}
		int size = Serial.available();
		if(size > sizeof(recvBuffer)-1)
			size = sizeof(recvBuffer)-1;
		if(size > 0) {
			Serial.read(recvBuffer, size);
			recvBuffer[size] = 0;
			onSerial(ControlId::SerialPort, now, recvBuffer, size);
			eventFired = true;
		}
	} else {
		if(_currentSerialConnect) {
			_currentSerialConnect = false;
			onSerialDisconnect(ControlId::SerialPort, now);
			eventFired = true;
		}
	}

	uint8_t lastButtonState = _currentButtonState;
	_currentButtonState = ButtonController::scan(now);
#if defined(JOYSTICK_CONTROLLER)
	uint8_t joystickFlag = JoystickController::scan(now);
#endif
	uint8_t lastWheelDelta = _currentWheelDelta;
	_currentWheelDelta = WheelController::scan(now);
	uint16_t lastKeyState = _currentKeyState;
	_currentKeyState = KeyboardController::scan(now);
	uint8_t lastRangingState = _currentRangingState;
	_currentRangingState = RangingController::scan(now);

#if defined(JOYSTICK_CONTROLLER)
	if(joystickFlag & 0x07) {
		float x = JoystickController::readAngle(0);
		float y = JoystickController::readAngle(1);
		float z = JoystickController::readAngle(2);
		if(currentJoystickButtonState == 0) {
			if(!(currentButtonState & JOYSTICK_SHIFT_BUTTON)) {
				currentJoystickState = 1; // rotate
				onStickBeginRotate(ControlId::Joystick, now);
				eventFired = true;
			} else {
				currentJoystickState = 2; // move
				onStickBeginMove(ControlId::Joystick, now);
				eventFired = true;
			}
		}
		if(currentJoystickState == 1) {
			currentJoystickX = x;
			currentJoystickY = y;
			currentJoystickZ = z;
			onStickRotate(ControlId::Joystick, now, x, y ,z);
			eventFired = true;
		} else if(currentJoystickState == 2) {
			currentJoystickMX = x;
			currentJoystickMY = y;
			currentJoystickMZ = z;
			onStickMove(ControlId::Joystick, now, x, y ,z);
			eventFired = true;
		}
	} else {
		if(currentJoystickState == 1) {
			onStickEndRotate(ControlId::Joystick, now);
			currentJoystickState = 0;
			eventFired = true;
		} else if(currentJoystickState == 2) {
			onStickEndMove(ControlId::Joystick, now);
			currentJoystickState = 0;
			eventFired = true;
		}
	}
#endif
	if(_currentWheelDelta != lastWheelDelta && _currentWheelDelta != 0) {
		if(!_currentWheelState) {
			onWheelBegin(ControlId::Wheel, now, _currentWheelDelta);
			_currentWheelState = true;
		}
		onWheel(ControlId::Wheel, now, _currentWheelDelta);
		_lastWheelTime = now;
		eventFired = true;
	}
	else if(_currentWheelState) {
		if(now - _lastWheelTime > 800000L) {
			onWheelEnd(ControlId::Wheel, now, _currentWheelDelta);
			eventFired = true;
			_lastWheelTime = now;
			_currentWheelState = false;
		}
	}

	if(_currentButtonState != lastButtonState) {
		uint8_t buttonBit = 1;
		int buttonId = ControlId::Button0;
		for(int i = 0; i < BUTTON_COUNT; i++) {
			if (_currentButtonState & buttonBit) {
				if (!(lastButtonState & buttonBit)) {
					onKeyDown((ControlId) buttonId, now);
					eventFired = true;
					_lastButtonDownTime = now;
				}
			} else {
				if (lastButtonState & buttonBit) {
					onKeyUp((ControlId) buttonId, now);
					eventFired = true;
					_lastButtonUpTime = now;
					if(_lastButtonUpTime - _lastButtonDownTime < 800000L)
						onKeyInput((ControlId)buttonId, now);
					else
						onKeyLongPress((ControlId)buttonId, now);
				}
			}
			buttonBit <<= 1;
			buttonId++;
		}
	}
	if(lastKeyState != _currentKeyState) {
		uint16_t keyBit = 1;
		int controlId = ControlId::KeySwitch0;
		for(int i = 0; i < KEYBOARD_KEY_COUNT; i++) {
			if(_currentKeyState & keyBit) {
				if(!(lastKeyState & keyBit)) {
					onKeyDown((ControlId) controlId, now);
					eventFired = true;
					_lastKeyDownTime = now;
				}
			} else {
				if (lastKeyState & keyBit) {
					onKeyUp((ControlId)controlId,now);
					eventFired = true;
					_lastKeyUpTime = now;
					if(_lastKeyUpTime - _lastKeyDownTime < 800000L)
						onKeyInput((ControlId)controlId, now);
					else
						onKeyLongPress((ControlId)controlId, now);
				}
			}
			keyBit <<= 1;
			controlId++;
		}
	}

	if(_currentRangingState < RANGE_THRESHOLD) {
		if(lastRangingState >= RANGE_THRESHOLD) {
			onEnter(ControlId::Ranging, now);
			eventFired = true;
		}
	} else {
		if(lastRangingState < RANGE_THRESHOLD) {
			onLeave(ControlId::Ranging, now);
			eventFired = true;
		}
	}

	if(eventFired) {
		_lastActionTime = millis();
		_lastActionElapsedTime = 0;
	} else {
		_lastActionElapsedTime = millis() - _lastActionTime;
	}
	onLoop(now);
}


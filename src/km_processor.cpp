//
// Created by yasuoki on 25/01/30.
//

#include "km_processor.h"
#include <Arduino.h>
//#include <USB.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include "driver.h"
#include "control/button_controller.h"
#include "control/keyboard_controller.h"
#include "control/display_controller.h"
#include "visual_manager.h"

Configuration *	KMProcessor::_config = nullptr;
uint16_t KMProcessor::_currentAppr = 0;
uint16_t KMProcessor::_currentPage = 0;
Application * KMProcessor::_currentApprPtr = nullptr;
Page * KMProcessor::_currentPagePtr = nullptr;
bool KMProcessor::_screenTimeout = false;
bool KMProcessor::_mouseTracking = false;
int32_t KMProcessor::_mouseTrackDistanceX = 0;
int32_t KMProcessor::_mouseTrackDistanceY = 0;

int16_t KMProcessor::_keySwitch0_tmv = 0;
int16_t KMProcessor::_keySwitch1_tmv = 0;
int16_t KMProcessor::_keySwitch2_tmv = 0;
int16_t KMProcessor::_keySwitch3_tmv = 0;
int16_t KMProcessor::_keySwitch4_tmv = 0;
int16_t KMProcessor::_keySwitch5_tmv = 0;
int16_t KMProcessor::_keySwitch6_tmv = 0;
int16_t KMProcessor::_keySwitch7_tmv = 0;
int16_t KMProcessor::_keySwitch8_tmv = 0;
int16_t KMProcessor::_keySwitch9_tmv = 0;
int16_t KMProcessor::_keySwitch10_tmv = 0;
int16_t KMProcessor::_keySwitch11_tmv = 0;
int16_t KMProcessor::_keySwitch12_tmv = 0;
int16_t KMProcessor::_keySwitch13_tmv = 0;
int16_t KMProcessor::_keySwitch14_tmv = 0;
int16_t KMProcessor::_keySwitch15_tmv = 0;
int16_t KMProcessor::_button0_tmv = 0;
int16_t KMProcessor::_button1_tmv = 0;
int16_t KMProcessor::_wheel_tmv = 0;
int16_t KMProcessor::_display_tmv = 0;
int16_t KMProcessor::_ranging_tmv = 0;
int16_t KMProcessor::_main_tmv = 0;

KMProcessor::KMProcessor() {
	ConfigLoader::init();
}

bool KMProcessor::init() {
	_config = ConfigLoader::load("config");
	if (_config == nullptr) {
		return false;
	}
	_currentAppr = 0;
	_currentPage = 0;
	_currentApprPtr = _config->root->appRef[_currentAppr].applicationPtr;
	_currentPagePtr = _currentApprPtr->pages[_currentPage].pagePtr;

	Driver::init();
	remapApplication();
	VisualManager::apply();
	return true;
}

void KMProcessor::remapApplication() {
	VisualManager::setVisual(ControlId::Display, _currentApprPtr->visualPtr);
	remapPage();
}

void KMProcessor::remapPage() {
	VisualManager::clearSubFrames();
	for(int i = 0; i < _currentPagePtr->count; i++) {
		ControlBind *bind = &_currentPagePtr->controlBind[i];
		if(bind->controlPtr->visualPtr != nullptr) {
			VisualManager::setVisual((ControlId)bind->controlId, bind->controlPtr->visualPtr);
		}
	}
}

void KMProcessor::changeApplication(uint16_t applicationIndex, uint16_t pageIndex) {
	_currentAppr = applicationIndex;
	_currentApprPtr = _config->root->appRef[_currentAppr].applicationPtr;
	_currentPage = pageIndex;
	_currentPagePtr = _currentApprPtr->pages[_currentPage].pagePtr;
	remapApplication();
	VisualManager::applyDisplay();
}

void KMProcessor::nextApplication() {
	uint16_t nextAppIndex = _currentAppr;
	if (nextAppIndex == _config->root->count - 1) {
		nextAppIndex = 0;
	} else {
		nextAppIndex++;
	}
	if(nextAppIndex != _currentAppr)
		changeApplication(nextAppIndex,0);
}

void KMProcessor::prevApplication() {
	uint16_t nextAppIndex = _currentAppr;
	if (nextAppIndex == 0) {
		nextAppIndex = _config->root->count - 1;
	} else {
		nextAppIndex--;
	}
	if(nextAppIndex != _currentAppr)
		changeApplication(nextAppIndex,0);
}

void KMProcessor::changePage(uint16_t pageIndex) {
	_currentPage = pageIndex;
	_currentPagePtr = _currentApprPtr->pages[_currentPage].pagePtr;
	remapPage();
	VisualManager::apply();
}

void KMProcessor::nextPage() {
	uint16_t nextPageIndex = _currentPage;
	if (nextPageIndex == _currentApprPtr->count - 1) {
		nextPageIndex = 0;
	} else {
		nextPageIndex++;
	}
	if(nextPageIndex != _currentPage)
		changePage(nextPageIndex);
}

void KMProcessor::prevPage() {
	uint16_t nextPageIndex = _currentPage;
	if (nextPageIndex == 0) {
		nextPageIndex = _currentApprPtr->count - 1;
	} else {
		nextPageIndex--;
	}
	if(nextPageIndex != _currentPage)
		changePage(nextPageIndex);
}

void KMProcessor::doSleep() {
	VisualManager::hide();
}

void KMProcessor::doWakeup() {
	VisualManager::show();
}

void KMProcessor::mouseMove(int32_t x, int32_t y) {
	while (x != 0 || y != 0) {
		int8_t dx;
		int8_t dy;
		if (x < INT8_MIN)
			dx = INT8_MIN;
		else if (x > INT8_MAX)
			dx = INT8_MAX;
		else
			dx = (int8_t) x;
		if (y < INT8_MIN)
			dy = INT8_MIN;
		else if (y > INT8_MAX)
			dy = INT8_MAX;
		else
			dy = (int8_t) y;
		Driver::mouseMove(dx, dy);
		x -= dx;
		y -= dy;
		delay(sqrt(dx * dx + dy * dy));
	}
	if(_mouseTracking) {
		_mouseTrackDistanceX += x;
		_mouseTrackDistanceY += y;
	}
}

int KMProcessor::intRefeerence(int v) {
	if(VariableId::KeySwitch0_Status <= (uint16_t)v && (uint16_t)v <= VariableId::Main_TimerData) {
		switch(v) {
			case VariableId::KeySwitch0_Status:
				return (_currentKeyState & 0x00000001) ? 1: 0;
			case VariableId::KeySwitch0_TimerData:
				return _keySwitch0_tmv;
			case VariableId::KeySwitch1_Status:
				return (_currentKeyState & 0x00000002) ? 1: 0;
			case VariableId::KeySwitch1_TimerData:
				return _keySwitch1_tmv;
			case VariableId::KeySwitch2_Status:
				return (_currentKeyState & 0x00000004) ? 1: 0;
			case VariableId::KeySwitch2_TimerData:
				return _keySwitch2_tmv;
			case VariableId::KeySwitch3_Status:
				return (_currentKeyState & 0x00000008) ? 1: 0;
			case VariableId::KeySwitch3_TimerData:
				return _keySwitch3_tmv;
			case VariableId::KeySwitch4_Status:
				return (_currentKeyState & 0x00000010) ? 1: 0;
			case VariableId::KeySwitch4_TimerData:
				return _keySwitch4_tmv;
			case VariableId::KeySwitch5_Status:
				return (_currentKeyState & 0x00000020) ? 1: 0;
			case VariableId::KeySwitch5_TimerData:
				return _keySwitch5_tmv;
			case VariableId::KeySwitch6_Status:
				return (_currentKeyState & 0x00000040) ? 1: 0;
			case VariableId::KeySwitch6_TimerData:
				return _keySwitch6_tmv;
			case VariableId::KeySwitch7_Status:
				return (_currentKeyState & 0x00000080) ? 1: 0;
			case VariableId::KeySwitch7_TimerData:
				return _keySwitch7_tmv;
			case VariableId::KeySwitch8_Status:
				return (_currentKeyState & 0x00000100) ? 1: 0;
			case VariableId::KeySwitch8_TimerData:
				return _keySwitch8_tmv;
			case VariableId::KeySwitch9_Status:
				return (_currentKeyState & 0x00000200) ? 1: 0;
			case VariableId::KeySwitch9_TimerData:
				return _keySwitch9_tmv;
			case VariableId::KeySwitch10_Status:
				return (_currentKeyState & 0x00000400) ? 1: 0;
			case VariableId::KeySwitch10_TimerData:
				return _keySwitch10_tmv;
			case VariableId::KeySwitch11_Status:
				return (_currentKeyState & 0x00000800) ? 1: 0;
			case VariableId::KeySwitch11_TimerData:
				return _keySwitch11_tmv;
			case VariableId::KeySwitch12_Status:
				return (_currentKeyState & 0x00001000) ? 1: 0;
			case VariableId::KeySwitch12_TimerData:
				return _keySwitch12_tmv;
			case VariableId::KeySwitch13_Status:
				return (_currentKeyState & 0x00002000) ? 1: 0;
			case VariableId::KeySwitch13_TimerData:
				return _keySwitch13_tmv;
			case VariableId::KeySwitch14_Status:
				return (_currentKeyState & 0x00004000) ? 1: 0;
			case VariableId::KeySwitch14_TimerData:
				return _keySwitch14_tmv;
			case VariableId::KeySwitch15_Status:
				return (_currentKeyState & 0x00008000) ? 1: 0;
			case VariableId::KeySwitch15_TimerData:
				return _keySwitch15_tmv;
			case VariableId::Button0_Status:
				return (_currentButtonState & 0x01) ? 1: 0;
			case VariableId::Button0_TimerData:
				return _button0_tmv;
			case VariableId::Button1_Status:
				return (_currentButtonState & 0x02) ? 1: 0;
			case VariableId::Button1_TimerData:
				return _button1_tmv;
			case VariableId::Wheel_Delta:
				return _currentWheelDelta;
			case VariableId::Wheel_TimerData:
				return _wheel_tmv;
			case VariableId::Display_TimerData:
				return _display_tmv;
			case VariableId::Ranging_Status:
				return _currentRangingState < RANGE_THRESHOLD ? 1: 0;
			case VariableId::Ranging_Distance:
				return _currentRangingState;
			case VariableId::Ranging_TimerData:
				return _ranging_tmv;
			case VariableId::Main_TimerData:
				return _main_tmv;
		}
	}
	return v;
}

float KMProcessor::floatRefeerence(float v) {
	if(VariableId::KeySwitch0_Status <= (uint16_t)v && (uint16_t)v <= VariableId::Main_TimerData) {
		uint16_t vd = (uint16_t)v;
		switch(vd) {
			case VariableId::KeySwitch0_Status:
				return (_currentKeyState & 0x00000001) ? 1: 0;
			case VariableId::KeySwitch0_TimerData:
				return _keySwitch0_tmv;
			case VariableId::KeySwitch1_Status:
				return (_currentKeyState & 0x00000002) ? 1: 0;
			case VariableId::KeySwitch1_TimerData:
				return _keySwitch1_tmv;
			case VariableId::KeySwitch2_Status:
				return (_currentKeyState & 0x00000004) ? 1: 0;
			case VariableId::KeySwitch2_TimerData:
				return _keySwitch2_tmv;
			case VariableId::KeySwitch3_Status:
				return (_currentKeyState & 0x00000008) ? 1: 0;
			case VariableId::KeySwitch3_TimerData:
				return _keySwitch3_tmv;
			case VariableId::KeySwitch4_Status:
				return (_currentKeyState & 0x00000010) ? 1: 0;
			case VariableId::KeySwitch4_TimerData:
				return _keySwitch4_tmv;
			case VariableId::KeySwitch5_Status:
				return (_currentKeyState & 0x00000020) ? 1: 0;
			case VariableId::KeySwitch5_TimerData:
				return _keySwitch5_tmv;
			case VariableId::KeySwitch6_Status:
				return (_currentKeyState & 0x00000040) ? 1: 0;
			case VariableId::KeySwitch6_TimerData:
				return _keySwitch6_tmv;
			case VariableId::KeySwitch7_Status:
				return (_currentKeyState & 0x00000080) ? 1: 0;
			case VariableId::KeySwitch7_TimerData:
				return _keySwitch7_tmv;
			case VariableId::KeySwitch8_Status:
				return (_currentKeyState & 0x00000100) ? 1: 0;
			case VariableId::KeySwitch8_TimerData:
				return _keySwitch8_tmv;
			case VariableId::KeySwitch9_Status:
				return (_currentKeyState & 0x00000200) ? 1: 0;
			case VariableId::KeySwitch9_TimerData:
				return _keySwitch9_tmv;
			case VariableId::KeySwitch10_Status:
				return (_currentKeyState & 0x00000400) ? 1: 0;
			case VariableId::KeySwitch10_TimerData:
				return _keySwitch10_tmv;
			case VariableId::KeySwitch11_Status:
				return (_currentKeyState & 0x00000800) ? 1: 0;
			case VariableId::KeySwitch11_TimerData:
				return _keySwitch11_tmv;
			case VariableId::KeySwitch12_Status:
				return (_currentKeyState & 0x00001000) ? 1: 0;
			case VariableId::KeySwitch12_TimerData:
				return _keySwitch12_tmv;
			case VariableId::KeySwitch13_Status:
				return (_currentKeyState & 0x00002000) ? 1: 0;
			case VariableId::KeySwitch13_TimerData:
				return _keySwitch13_tmv;
			case VariableId::KeySwitch14_Status:
				return (_currentKeyState & 0x00004000) ? 1: 0;
			case VariableId::KeySwitch14_TimerData:
				return _keySwitch14_tmv;
			case VariableId::KeySwitch15_Status:
				return (_currentKeyState & 0x00008000) ? 1: 0;
			case VariableId::KeySwitch15_TimerData:
				return _keySwitch15_tmv;
			case VariableId::Button0_Status:
				return (_currentButtonState & 0x01) ? 1: 0;
			case VariableId::Button0_TimerData:
				return _button0_tmv;
			case VariableId::Button1_Status:
				return (_currentButtonState & 0x02) ? 1: 0;
			case VariableId::Button1_TimerData:
				return _button1_tmv;
			case VariableId::Wheel_Delta:
				return _currentWheelDelta;
			case VariableId::Wheel_TimerData:
				return _wheel_tmv;
			case VariableId::Display_TimerData:
				return _display_tmv;
			case VariableId::Ranging_Status:
				return _currentRangingState < RANGE_THRESHOLD ? 1: 0;
			case VariableId::Ranging_Distance:
				return _currentRangingState;
			case VariableId::Ranging_TimerData:
				return _ranging_tmv;
			case VariableId::Main_TimerData:
				return _main_tmv;
		}
	}
	return v;
}

void KMProcessor::cmdMouseMove(uint32_t now, ControlId control, EventId event, Command *command) {
	int16_t x = (int16_t)intRefeerence(command->param.mouseMoveCommand.x);
	int16_t y = (int16_t)intRefeerence(command->param.mouseMoveCommand.y);
	mouseMove(x,y);
}

void KMProcessor::cmdMouseTrackingStart(uint32_t now, ControlId control, EventId event, Command *command) {
	_mouseTracking = true;
	_mouseTrackDistanceX = 0;
	_mouseTrackDistanceY = 0;
}

void KMProcessor::cmdMouseTrackingStop(uint32_t now, ControlId control, EventId event, Command *command) {
	_mouseTracking = false;
}

void KMProcessor::cmdMouseTrackingRewind(uint32_t now, ControlId control, EventId event, Command *command) {
	int32_t x = -_mouseTrackDistanceX;
	int32_t y = -_mouseTrackDistanceY;
	_mouseTracking = false;
	_mouseTrackDistanceX = 0;
	_mouseTrackDistanceY = 0;
	mouseMove(x, y);
}

void KMProcessor::cmdMouseWheel(uint32_t now, ControlId control, EventId event, Command *command) {
	int16_t delta = (int16_t)intRefeerence(command->param.mouseWheelCommand.delta);
	while (delta != 0) {
		int8_t d;
		if (delta < INT8_MIN)
			d = INT8_MIN;
		else if (delta > INT8_MAX)
			d = INT8_MAX;
		else
			d = (int8_t) delta;
		Driver::mouseWheel(d);
		delta -= d;
		delay(d * 50);
	}
}

void KMProcessor::cmdMouseClick(uint32_t now, ControlId control, EventId event, Command *command) {
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(20);
}

void KMProcessor::cmdMouseDoubleClick(uint32_t now, ControlId control, EventId event, Command *command) {
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(200);
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(20);
}

void KMProcessor::cmdMouseTripleClick(uint32_t now, ControlId control, EventId event, Command *command) {
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(200);
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(200);
	Driver::mouseButtonPress(command->param.buttonCommand.button);
	delay(100);
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
	delay(20);
}

void KMProcessor::cmdButtonPress(uint32_t now, ControlId control, EventId event, Command *command) {
	Driver::mouseButtonPress(command->param.buttonCommand.button);
}

void KMProcessor::cmdButtonRelease(uint32_t now, ControlId control, EventId event, Command *command) {
	Driver::mouseButtonRelease(command->param.buttonCommand.button);
}

void KMProcessor::cmdKeyPress(uint32_t now, ControlId control, EventId event, Command *command) {
	UsageList *up = command->param.keyCommand.usageListPtr;
	if (up) {
		uint8_t *ptr = up->usage;
		for (int i = 0; i < up->len; i++) {
			Driver::keyPress(*ptr);
			delay(40);
			ptr++;
		}
	}
}

void KMProcessor::cmdKeyRelease(uint32_t now, ControlId control, EventId event, Command *command) {
	UsageList *up = command->param.keyCommand.usageListPtr;
	if (up) {
		uint8_t *ptr = up->usage;
		for (int i = 0; i < up->len; i++) {
			Driver::keyRelease(*ptr);
			delay(40);
			ptr++;
		}
	}
}

void KMProcessor::cmdKeyInput(uint32_t now, ControlId control, EventId event, Command *command) {
	UsageList *up = command->param.keyCommand.usageListPtr;
	if (up) {
		uint8_t *ptr = up->usage;
		for (int i = 0; i < up->len; i++) {
			Driver::keyPress(*ptr);
			if (!IS_MODIFIER_KEY(*ptr)) {
				delay(20);
				Driver::keyRelease(*ptr);
			}
			delay(20);
			ptr++;
		}
	}
	Driver::releaseAllKey();
}

bool KMProcessor::compaireKey(float key, uint16_t op, float value)
{
	switch(op) {
		case MapComparator::Less:
			return key < value;
		case MapComparator::LessOrEqual:
			return key <= value;
		case MapComparator::Equal:
			return key == value;
		case MapComparator::NotEqual:
			return key != value;
		case MapComparator::GreaterOrEqual:
			return key >= value;
		case MapComparator::Greater:
			return key > value;
	}
	return false;
}

void KMProcessor::cmdMapInput(uint32_t now, ControlId control, EventId event, Command *command) {
	MapInputCommand *p = &command->param.mapInputCommand;
	float key = floatRefeerence(p->key);
	MapInputData* data = p->mapData;
	for(int i = 0; i < p->count; i++) {
		if(compaireKey(key, data->op, data->compData)) {
			UsageList *up = data->usageListPtr;
			if (up) {
				uint8_t *ptr = up->usage;
				for (int j = 0; j < up->len; j++) {
					Driver::keyPress(*ptr);
					if (!IS_MODIFIER_KEY(*ptr)) {
						delay(20);
						Driver::keyRelease(*ptr);
					}
					delay(20);
					ptr++;
				}
			}
			break;
		}
		data++;
	}
}

void KMProcessor::cmdMapAction(uint32_t now, ControlId control, EventId event, Command *command) {
	MapActionCommand *p = &command->param.mapActionCommand;
	float key = floatRefeerence(p->key);
	MapActionData * data = p->mapData;
	for(int i = 0; i < p->count; i++) {
		if(compaireKey(key, data->op, data->compData)) {
			ActionCommand *actionCommand = data->actionCommandPtr;
			if (actionCommand != nullptr) {
				for (int k = 0; k < actionCommand->count; k++) {
					Command *command = actionCommand->commandPtr[k];
					doCommand(now, control, event, command);
				}
			}
			break;
		}
		data++;
	}
}

void KMProcessor::cmdPageChange(uint32_t now, ControlId control, EventId event, Command *command) {
	int nextPage = -1;
	for (int i = 0; i < _currentApprPtr->count; i++) {
		if (strcmp(command->param.pageChangeCommand.pageNamePtr, _currentApprPtr->pages[i].pageNamePtr) == 0) {
			nextPage = i;
			break;
		}
	}
	if(nextPage != _currentPage)
		changePage(nextPage);
}

void KMProcessor::cmdApplicationChange(uint32_t now, ControlId control, EventId event, Command *command) {
	ApplicationChangeCommand *p = &command->param.applicationChangeCommand;
	int nextAppr = -1;
	for (int i = 0; i < _config->root->count; i++) {
		if (strcmp(p->applicationNamePtr, _config->root->appRef[i].appNamePtr) == 0) {
			nextAppr = i;
			break;
		}
	}
	if(nextAppr != -1) {
		int nextPage = -1;
		if(p->pageNamePtr != nullptr) {
			Application *appr = _config->root->appRef[nextAppr].applicationPtr;
			for (int i = 0; i < appr->count; i++) {
				if (strcmp(p->pageNamePtr, appr->pages[i].pageNamePtr) == 0) {
					nextPage = i;
					break;
				}
			}
			if(nextPage == -1)
				return;
		}
		if(nextPage == -1)
			nextPage = 0;
		if(nextAppr != _currentAppr || nextPage != _currentPage) {
			changeApplication(nextAppr, nextPage);
		}
	}
}

void KMProcessor::cmdDelay(uint32_t now, ControlId control, EventId event, Command *command) {
	uint16_t timeout = command->param.delayCommand.timeout;
	delay(timeout);
}

void KMProcessor::doCommand(uint32_t now, ControlId control, EventId event, Command *command) {
	switch (command->commandId) {
		case CommandId::MouseMove:
			cmdMouseMove(now, control, event, command);
			break;
		case CommandId::MouseTrackingStart:
			cmdMouseTrackingStart(now, control, event, command);
			break;
		case CommandId::MouseTrackingStop:
			cmdMouseTrackingStop(now, control, event, command);
			break;
		case CommandId::MouseTrackingRewind:
			cmdMouseTrackingRewind(now, control, event, command);
			break;
		case CommandId::MouseWheel:
			cmdMouseWheel(now, control, event, command);
			break;
		case CommandId::MouseClick:
			cmdMouseClick(now, control, event, command);
			break;
		case CommandId::MouseDoubleClick:
			cmdMouseDoubleClick(now, control, event, command);
			break;
		case CommandId::MouseTripleClick:
			cmdMouseTripleClick(now, control, event, command);
			break;
		case CommandId::ButtonPress:
			cmdButtonPress(now, control, event, command);
			break;
		case CommandId::ButtonRelease:
			cmdButtonRelease(now, control, event, command);
			break;
		case CommandId::KeyPress:
			cmdKeyPress(now, control, event, command);
			break;
		case CommandId::KeyRelease:
			cmdKeyRelease(now, control, event, command);
			break;
		case CommandId::KeyInput:
			cmdKeyInput(now, control, event, command);
			break;
		case CommandId::MapInput:
			cmdMapInput(now, control, event, command);
			break;
		case CommandId::MapAction:
			cmdMapAction(now, control, event, command);
			break;
		case CommandId::PageChange:
			cmdPageChange(now, control, event, command);
			break;
		case CommandId::ApplicationChange:
			cmdApplicationChange(now, control, event, command);
			break;
	}
}

void KMProcessor::eventDispatch(uint32_t now, ControlId control, EventId event) {
	if (_currentRangingState < RANGE_THRESHOLD)
		return;
	//Serial.printf("event dispatch: c=%x e=%x\r\n", control, event);
	for (int i = 0; i < _currentPagePtr->count; i++) {
		ControlBind *ctrlBind = &_currentPagePtr->controlBind[i];
		if (ctrlBind->controlId == control) {
			//Serial.println("control match");
			Control *controlPtr = ctrlBind->controlPtr;
			for (int j = 0; j < controlPtr->count; j++) {
				EventBind *eventBind = &controlPtr->eventBind[j];
				if (eventBind->eventId == event) {
					//Serial.println("event match");
					ActionCommand *actionCommand = eventBind->actionCommandPtr;
					if (actionCommand != nullptr) {
						for (int k = 0; k < actionCommand->count; k++) {
							Command *command = actionCommand->commandPtr[k];
							//Serial.printf("action [%d] %x\r\n", k, command->commandId);
							doCommand(now, control, event, command);
						}
					}
				}
			}
		}
	}
	//Serial.println("dispatch end");
}

void KMProcessor::onKeyDown(ControlId from, uint32_t now) {
	if (_currentRangingState < RANGE_THRESHOLD) {
		if (from == ControlId::Button0) {
			nextApplication();
			return;
		} else if (from == ControlId::Button1) {
			prevApplication();
			return;
		}
	}
	eventDispatch(now, from, EventId::KeyDownEvent);
}

void KMProcessor::onKeyUp(ControlId from, uint32_t now) {
	eventDispatch(now, from, EventId::KeyUpEvent);
}

void KMProcessor::onKeyInput(ControlId from, uint32_t now) {
	eventDispatch(now, from, EventId::KeyInputEvent);
}

void KMProcessor::onKeyLongPress(ControlId from, uint32_t now) {
	eventDispatch(now, from, EventId::LongPressEvent);
}

void KMProcessor::onWheelBegin(ControlId from, uint32_t now, int8_t w) {
	eventDispatch(now, from, EventId::BeginWheelEvent);
}

void KMProcessor::onWheel(ControlId from, uint32_t now, int8_t w) {
    eventDispatch(now, from, EventId::WheelEvent);
}

void KMProcessor::onWheelEnd(ControlId from, uint32_t now, int8_t w) {
	eventDispatch(now, from, EventId::EndWheelEvent);
}

void KMProcessor::onEnter(ControlId from, uint32_t now) {
	VisualManager::hideLed();
	eventDispatch(now, from, EventId::EnterEvent);
}

void KMProcessor::onLeave(ControlId from, uint32_t now) {
	VisualManager::showLed();
	VisualManager::apply();
	eventDispatch(now, from, EventId::LeaveEvent);
}

void KMProcessor::onUsbMount(ControlId from, uint32_t now) {
	doWakeup();
}

void KMProcessor::onUsbUnmount(ControlId from, uint32_t now) {
	doSleep();
}

void KMProcessor::onLoop(uint32_t now) {
	if (_lastActionElapsedTime > 30L * 60L * 1000L) {
		if (!_screenTimeout) {
			doSleep();
			_screenTimeout = true;
		}
	} else {
		if (_screenTimeout) {
			doWakeup();
			_screenTimeout = false;
		}
	}
}

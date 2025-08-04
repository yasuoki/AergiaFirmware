//
// Created by Yasuoki on 2025/05/27.
//

#include <Arduino.h>

#include "config.h"
#include "visual_manager.h"
#include "visual_renderer.h"
#include "control/display_controller.h"
#include "control/neopixel_controller.h"
#if defined(USE_SSD1306_DRIVER_ARRAY)
#include "i2c_multiplexer.h"
#endif

#define VISUAL_FRAME_COUNT 24

Visual			_emptyVisual;
VisualFrame 	_visualMapping[VISUAL_FRAME_COUNT];

ControlId		_neopixelLedMap[] = NEO_PIXEL_LED_MAP;
#if defined(USE_SSD1306_DRIVER_ARRAY)
ControlId		_screenArrayMap[] = SSD1306_MAP;
#else
ControlId		_subFrameMap[] = SUBFRAME_MAP;
#endif

void VisualManager::mappingFrames()
{
#if defined(USE_SSD1306_DRIVER_ARRAY)
	for(int i = 0; i < SSD1306_ARRAY_COUNT; i++) {
		ControlId id = _screenArrayMap[i];
		int index = id & 0xff;
		if(0 <= index && index < VISUAL_FRAME_COUNT) {
			VisualFrame *vfPtr = &_visualMapping[index];
			vfPtr->controlId = id;
			vfPtr->screen = (int16_t)(i+1);
			vfPtr->frame = 0;
		}
	}
#else
	for(int i = 0; i < SUBFRAME_COUNT; i++) {
		ControlId id = _subFrameMap[i];
		int index = id & 0xff;
		if(0 <= index && index < VISUAL_FRAME_COUNT) {
			VisualFrame *vfPtr = &_visualMapping[index];
			vfPtr->controlId = id;
			vfPtr->screen = 0;
			vfPtr->frame = (int16_t)(i+1);
		}
	}
#endif
}

void VisualManager::mappingNeoPixelArray() {
	for(int i = 0; i < NEO_PIXEL_LED_COUNT; i++) {
		ControlId controlId = _neopixelLedMap[i];
		int index = controlId & 0xff;
		if(0 <= index && index < VISUAL_FRAME_COUNT) {
			VisualFrame *vfPtr = &_visualMapping[index];
			vfPtr->ledIndex = (int16_t)i;
		}
	}
}

bool VisualManager::init()
{
	_emptyVisual.text = nullptr;
	_emptyVisual.icon = 0;
	_emptyVisual.bgColor = 0;
	_emptyVisual.color = 0xffffff;
	_emptyVisual.fontSize = 1;

	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		vfPtr->controlId = (ControlId)0;
		vfPtr->screen = -1;
		vfPtr->frame = -1;
		vfPtr->ledIndex = -1;
		vfPtr->visual = _emptyVisual;
		vfPtr->apply = false;
	}

	VisualFrame *mainVisualFrame = getVisualFrame(ControlId::Display);
	mainVisualFrame->controlId = ControlId::Display;
	mainVisualFrame->screen = 0;
	mainVisualFrame->frame = 0;
	mainVisualFrame->ledIndex = -1;
	mainVisualFrame->apply = false;

	mappingFrames();
	mappingNeoPixelArray();
	return true;
}

VisualFrame* VisualManager::getVisualFrame(ControlId controlId)
{
	int index = controlId & 0xff;
	if(0 <= index && index < VISUAL_FRAME_COUNT) {
		return &_visualMapping[index];
	}
	return nullptr;
}

void VisualManager::clear()
{
	VisualFrame *bf = _visualMapping;
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		bf->visual = _emptyVisual;
		bf->apply = false;
		bf++;
	}
}
void VisualManager::clearSubFrames() {
	VisualFrame *bf = _visualMapping;
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		if(bf->controlId != ControlId::Display) {
			bf->visual = _emptyVisual;
			bf->apply = false;
		}
		bf++;
	}
}

void VisualManager::setVisual(ControlId controlId, Visual *visual) {
	VisualFrame *vfPtr = getVisualFrame(controlId);

	if(vfPtr != nullptr) {
		if(visual != nullptr) {
			vfPtr->visual = *visual;
		} else {
			vfPtr->visual = _emptyVisual;
		}
		vfPtr->apply = false;
	}
}

void VisualManager::updateVisual(ControlId controlId, Visual *visual) {
	VisualFrame *vfPtr = getVisualFrame(controlId);

	if(vfPtr != nullptr) {
		if(visual->text != nullptr)
			vfPtr->visual.text = visual->text;
		if(visual->icon != 0)
			vfPtr->visual.icon = visual->icon;
		if(visual->color != 0xffffffff)
			vfPtr->visual.color = visual->color;
		if(visual->bgColor != 0xffffffff)
			vfPtr->visual.bgColor = visual->bgColor;
		if(visual->fontSize != 0)
			vfPtr->visual.fontSize = visual->fontSize;
		vfPtr->apply = false;
	}
}

bool VisualManager::apply() {
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		if(VisualRenderer::applyVisual(vfPtr))
			return true;
	}
	NeoPixelController::showNeoPixels();
	return false;
}

void VisualManager::applyImmideate() {
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		VisualRenderer::applyVisual(vfPtr);
	}
	NeoPixelController::showNeoPixels();
}

void VisualManager::applyDisplay() {
	VisualFrame *vfPtr = getVisualFrame(ControlId::Display);
	if(vfPtr != nullptr) {
		VisualRenderer::applyVisual(vfPtr);
	}
}

void VisualManager::show() {
	DisplayController::selectScreen(0)->screenOn();
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		VisualRenderer::show(vfPtr);
	}
	NeoPixelController::showNeoPixels();
}

void VisualManager::hide() {
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		VisualRenderer::hide(vfPtr);
	}
	NeoPixelController::clearNeoPixels();
	DisplayController::selectScreen(0)->screenOff();
}

void VisualManager::hideLed() {
	NeoPixelController::clearNeoPixels();
}

void VisualManager::showLed() {
	for(int i = 0; i < VISUAL_FRAME_COUNT; i++) {
		VisualFrame *vfPtr = &_visualMapping[i];
		if(vfPtr->ledIndex != -1) {
			NeoPixelController::setNeoPixelColor(vfPtr->ledIndex, vfPtr->visual.bgColor);
		}
	}
	NeoPixelController::showNeoPixels();
}

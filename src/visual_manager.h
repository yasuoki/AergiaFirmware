//
// Created by Yasuoki on 2025/05/27.
//

#ifndef INC_AERGIA_VISUAL_MANAGER_H
#define INC_AERGIA_VISUAL_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "visual_renderer.h"

class VisualManager {
private:
	static void mappingFrames();
	static void mappingNeoPixelArray();

public:
	static bool init();
	static VisualFrame * getVisualFrame(ControlId controlId);
	static void clear();
	static void clearSubFrames();
	static void setVisual(ControlId controlId, Visual *visual);
	static void updateVisual(ControlId controlId, Visual *visual);
	static void apply();
	static void applyDisplay();
	static void show();
	static void hide();
	static void showLed();
	static void hideLed();
};

#endif //INC_AERGIA_VISUAL_MANAGER_H

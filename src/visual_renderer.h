//
// Created by Yasuoki on 2025/05/27.
//

#ifndef INC_AERGIA_VISUAL_RENDERER_H
#define INC_AERGIA_VISUAL_RENDERER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

#include "config.h"
#include "control/display_controller.h"

typedef struct _VisualFrame {
	ControlId		controlId;
	int16_t 		screen;
	int16_t 		frame;
	int16_t 		ledIndex;
	Visual 			visual;
	bool			apply;
} VisualFrame;

typedef struct _IconData {
	uint16_t	width;
	uint16_t	height;
	uint16_t	format;
	uint8_t 	bmp[1];
} IconData;

typedef struct _FrameRect {
	int16_t 	x;
	int16_t 	y;
	uint16_t 	width;
	uint16_t 	height;
} FrameRect;

class VisualRenderer {
public:
	static void init() {}

	static void getFrameRect(VisualFrame *vfPtr, FrameRect *rectPtr);
	static IconData *loadIconFile(uint16_t iconIndex);
	static void setLargeFont(ScreenGFX *screen);
	static void setSmallFont(ScreenGFX *screen);

	static void drawText(ScreenGFX *screen, int16_t yp, const char *text);
	static void drawIcon(ScreenGFX *screen, const IconData *icon);
	static void clear(ScreenGFX *screen);

	static void drawFrameText(VisualFrame *vfPtr, const char *text);
	static void drawFrameIcon(VisualFrame *vfPtr, const IconData *icon);
	static void clearFrame(VisualFrame *frame);

	static void drawInitializeScreen();
	static bool applyVisual(VisualFrame *frame);
	static void drawMainScreen(VisualFrame *frame);
	static void drawControlFrame(VisualFrame *frame);
	static void show(VisualFrame *frame);
	static void hide(VisualFrame *frame);
	static void showLed(VisualFrame *frame);
	static void hideLed(VisualFrame *frame);
};

#endif //INC_AERGIA_VISUAL_RENDERER_H

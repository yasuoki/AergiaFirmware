//
// Created by Yasuoki on 2025/05/27.
//

#include "visual_renderer.h"
#include "control/display_controller.h"
#include "control/neopixel_controller.h"
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <FS.h>
#include <LittleFS.h>

/**
 * Retrieves the rectangular frame dimensions of a visual frame and populates the specified FrameRect object.
 *
 * This function calculates the rectangle dimensions for a specific frame within a screen, based on the
 * frame position and screen resolution. The calculations differ depending on whether the code is compiled
 * with the `USE_SSD1306_DRIVER_ARRAY` macro.
 *
 * @param vfPtr Pointer to a VisualFrame object that contains information about the frame to calculate dimensions for.
 * @param rectPtr Pointer to a FrameRect object where the resulting rectangle dimensions will be stored.
 *
 * - If `USE_SSD1306_DRIVER_ARRAY` is defined:
 *   The entire screen dimensions are assigned to the rectangle.
 * - If `USE_SSD1306_DRIVER_ARRAY` is not defined:
 *   The rectangle dimensions are calculated based on sub-frame width, height, and frame positioning
 *   within the screen.
 */
void VisualRenderer::getFrameRect(VisualFrame *vfPtr, FrameRect *rectPtr) {
	ScreenGFX *screen = DisplayController::getScreen(vfPtr->screen);
#if defined(USE_SSD1306_DRIVER_ARRAY)
	rectPtr->x = 0;
	rectPtr->y = 0;
	rectPtr->width = screen->gfx->width();
	rectPtr->height = screen->gfx->height();
#else
	int cn = vfPtr->frame - 1;
	int col = cn % 4;
	int row = cn / 4;
	int left = (screen->gfx->width() - SUBFRAME_WIDTH * 4) / 2;
	int top = (screen->gfx->height() - SUBFRAME_HEIGHT * 4) / 2;
	rectPtr->x = (int16_t)(left + col * SUBFRAME_WIDTH);
	rectPtr->y = (int16_t)(top + row * SUBFRAME_HEIGHT);
	rectPtr->width = SUBFRAME_WIDTH;
	rectPtr->height = SUBFRAME_HEIGHT;
#endif
}

/**
 * @brief Loads an icon file from the file system given its index and returns its data.
 *
 * This function constructs a file name from the provided icon index, attempts
 * to open the file from the LittleFS file system, and reads its content into
 * dynamically allocated memory. If the file cannot be opened, the function
 * returns a nullptr. The caller is responsible for freeing the allocated memory
 * once it is no longer needed.
 *
 * @param iconIndex The index of the icon file to load.
 * @return Pointer to an IconData structure containing the icon data.
 *         Returns nullptr if the file cannot be opened.
 */
IconData* VisualRenderer::loadIconFile(uint16_t iconIndex)
{
	char fileName[16];
	sprintf(fileName, "icon%u", iconIndex);
	File file = LittleFS.open(fileName, "r");
	if(!file) {
		return nullptr;
	}
	uint16_t size = file.size();
	IconData *icon = (IconData*)malloc(size);
	file.read((uint8_t *)icon, size);
	file.close();
	return icon;
}

/**
 * @brief Sets the large font for the specified `ScreenGFX` instance.
 *
 * This method configures the `ScreenGFX` object to use the `FreeSans18pt7b` font
 * from the Adafruit GFX library. This is typically used to render text in a larger font size.
 *
 * @param screen A pointer to the `ScreenGFX` instance where the large font will be set.
 */
void VisualRenderer::setLargeFont(ScreenGFX *screen)
{
	screen->gfx->setFont(&FreeSans18pt7b);
}

/**
 * @brief Sets the small font for the specified `ScreenGFX` instance.
 *
 * This method configures the `ScreenGFX` object to use the `FreeSans12pt7b` font
 * from the Adafruit GFX library. This is typically used to render text in a small font size.
 *
 * @param screen A pointer to the `ScreenGFX` instance where the small font will be set.
 */
void VisualRenderer::setSmallFont(ScreenGFX *screen)
{
	screen->gfx->setFont(&FreeSans12pt7b);
}

void VisualRenderer::clear(ScreenGFX *screen) {
	screen->clear();
}

void VisualRenderer::drawText(ScreenGFX *screen, int16_t yp, const char *text) {
	int16_t x, y;
	uint16_t w, h;
	screen->gfx->getTextBounds(text, 0, 0, &x, &y, &w, &h);
	int16_t xp = (int16_t) ((screen->gfx->width() - w) / 2 + screen->offsetX());
	screen->gfx->setCursor(xp, (int16_t)(yp+screen->offsetY()));
	screen->gfx->println(text);
}

void VisualRenderer::drawIcon(ScreenGFX *screen, const IconData *icon)
{
	int xp = (screen->gfx->width() - icon->width) / 2 + screen->offsetX();
	int yp = (screen->gfx->height() - icon->height) / 2 + screen->offsetY();
	if(icon->format == PixelFormat::Rgb565)
		screen->gfx->drawRGBBitmap((int16_t )xp, (int16_t)yp, (uint16_t*)&icon->bmp, icon->width, icon->height);
	else
		screen->gfx->drawBitmap((int16_t )xp, (int16_t)yp,icon->bmp, (int16_t)icon->width, (int16_t)icon->height, 1);
}


/**
 * Clears a specified frame on a screen.
 *
 * This function clears the rendering frame defined by the given VisualFrame structure.
 * It first verifies that the provided screen and frame are valid. If the frame
 * is not specified (frame == 0), the entire screen is cleared. Otherwise, it calculates
 * the specific rectangular area of the frame that needs to be cleared and fills it with a
 * default background color (0).
 *
 * @param vfPtr Pointer to a VisualFrame structure specifying the screen and frame to clear.
 *              - `vfPtr->screen` specifies the screen number to operate on. If `screen == -1`, the operation is skipped.
 *              - `vfPtr->frame` specifies the frame within the screen to clear. If `frame == 0`, the entire screen is cleared.
 */
void VisualRenderer::clearFrame(VisualFrame *vfPtr) {

	if(vfPtr->screen == -1)
		return;

	ScreenGFX *screen = DisplayController::selectScreen(vfPtr->screen);
	if(screen == nullptr)
		return;

	if(vfPtr->frame == 0) {
		screen->clear();
		return;
	}
	FrameRect rect;
	getFrameRect(vfPtr, &rect);
	screen->gfx->fillRect(rect.x, rect.y, (int16_t)rect.width, (int16_t)rect.height, 0);
}

void VisualRenderer::drawFrameText(VisualFrame *vfPtr, const char *text) {
	if(vfPtr->screen == -1)
		return;
	ScreenGFX *screen = DisplayController::selectScreen(vfPtr->screen);
	FrameRect rect;
	getFrameRect(vfPtr, &rect);
	int16_t x, y;
	uint16_t w, h;
	screen->gfx->getTextBounds(text, 0, 0, &x, &y, &w, &h);
	int16_t xp = (int16_t)((rect.width - w) / 2 + rect.x + screen->offsetX());
	int16_t yp = (int16_t)((rect.height - h) / 2 + h + rect.y + screen->offsetY());
	screen->gfx->setCursor(xp, yp);
	screen->gfx->print(text);
	screen->display();
}

void VisualRenderer::drawFrameIcon(VisualFrame *vfPtr, const IconData *icon) {
	if(vfPtr->screen == -1)
		return;
	ScreenGFX *screen = DisplayController::selectScreen(vfPtr->screen);
	FrameRect rect;
	getFrameRect(vfPtr, &rect);
	int xp = (rect.width - icon->width) / 2 + rect.x + screen->offsetX();
	int yp = (rect.height - icon->height) / 2 + rect.y + screen->offsetY();
	if(icon->format == PixelFormat::Rgb565)
		screen->gfx->drawRGBBitmap((int16_t)xp, (int16_t)yp, (uint16_t*)&icon->bmp, icon->width, icon->height);
	else
		screen->gfx->drawBitmap((int16_t)xp, (int16_t)yp, icon->bmp, (int16_t)icon->width, (int16_t)icon->height, 1);
}


void VisualRenderer::drawInitializeScreen() {
	ScreenGFX *screen = DisplayController::selectScreen(0);
	uint16_t darkGreen = DisplayController::rgb565Color(0x00, 0x64, 0x00);
	screen->gfx->setTextColor(darkGreen);
	screen->gfx->setTextSize(1);
	screen->gfx->fillScreen(0);
	screen->gfx->drawCircle(120,120,110,darkGreen);
	setSmallFont(screen);
	drawText(screen, 68, "initialize ...");
	drawText(screen,190, "Aergia " MODEL);
	screen->display();
}

uint16_t appColor = 0;
uint16_t appBgColor = 0;
void VisualRenderer::drawMainScreen(VisualFrame *vf) {
	ScreenGFX *screen = DisplayController::selectScreen(vf->screen);
	if(screen == nullptr) {
		return;
	}
	const Visual *visual = &vf->visual;
	appColor = DisplayController::rgb565Color(0x00, 0x64, 0x00);
	if (visual->color != 0xffffffff) {
		appColor = visual->color;
	}
	appBgColor = DisplayController::rgb565Color(0x00, 0x00, 0x00);
	if (visual->bgColor != 0xffffffff) {
		appBgColor = visual->bgColor;
	}
	screen->gfx->setTextColor(appColor);
	screen->gfx->setTextSize(1);
	screen->gfx->fillScreen(appBgColor);
	screen->gfx->drawCircle(120, 120, 110, appColor);
	setSmallFont(screen);
	drawText(screen, 190, DEVICE_MODEL);
	if (visual->icon != 0) {
		if (visual->text != nullptr) {
			drawText(screen, 52, visual->text);
		}
		IconData *icon = loadIconFile(visual->icon);
		if(icon != nullptr) {
			drawIcon(screen,icon);
			free(icon);
		}
	} else {
		if (visual->text != nullptr) {
			setLargeFont(screen);
			drawText(screen, 134, visual->text);
		}
	}
	screen->display();
}

void VisualRenderer::drawControlFrame(VisualFrame *vf) {
	ScreenGFX *screen = DisplayController::selectScreen(vf->screen);
	if(screen == nullptr) {
		return;
	}

	const Visual *visual = &vf->visual;
	if (visual->fontSize != 0) {
		if (visual->fontSize == 1) {
			setSmallFont(screen);
		} else if(visual->fontSize == 2) {
			setLargeFont(screen);
		}
	}
	if (visual->icon != 0) {
		clearFrame(vf);
		IconData *icon = loadIconFile(visual->icon);
		if(icon != nullptr) {
			drawFrameIcon(vf, icon);
			free(icon);
		}
	} else if (visual->text != nullptr) {
		clearFrame(vf);
		drawFrameText(vf, visual->text);
	} else {
		clearFrame(vf);
	}
#if !defined(USE_SSD1306_DRIVER_ARRAY)
	FrameRect rect;
	getFrameRect(vf, &rect);
	screen->gfx->drawRect(rect.x, rect.y, rect.width, rect.height, appBgColor);
#endif
	if (vf->ledIndex != -1 && visual->bgColor != 0xffffffff) {
		NeoPixelController::setNeoPixelColor(vf->ledIndex, visual->bgColor);
	}
	screen->display();
}

bool VisualRenderer::applyVisual(VisualFrame *vf)
{
	if(vf->screen == -1 || vf->apply)
		return false;
	if(vf->controlId == ControlId::Display) {
		drawMainScreen(vf);
	} else {
		drawControlFrame(vf);
	}
	vf->apply = true;
	return true;
}

void VisualRenderer::show(VisualFrame *vf) {
	if(vf->screen == -1)
		return;
	if(vf->controlId == ControlId::Display) {
		drawMainScreen(vf);
	} else {
		drawControlFrame(vf);
	}
	vf->apply = true;
}

void VisualRenderer::hide(VisualFrame *vf) {
	if(vf->screen == -1)
		return;
	ScreenGFX *screen = DisplayController::selectScreen(vf->screen);
	if(screen == nullptr) {
		return;
	}
	if(vf->controlId == ControlId::Display) {
		screen->screenOff();
	} else {
		screen->clear();
		screen->display();
	}
	if(vf->ledIndex != -1) {
		NeoPixelController::setNeoPixelColor(vf->ledIndex,0);
	}
}


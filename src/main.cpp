#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "i2c_multiplexer.h"
#include "control/neopixel_controller.h"
#include "control/button_controller.h"
#include "control/display_controller.h"
#include "control/keyboard_controller.h"
#include "control/wheel_controller.h"
#include "control/ranging_controller.h"
#include "visual_manager.h"
#include "visual_renderer.h"

#include "processor.h"

Processor* processor;

void setup() {
	Wire.setSDA(PIN_I2C_SDA);
	Wire.setSCL(PIN_I2C_SCL);
    Wire.begin();

	DisplayController::init();
	VisualRenderer::drawInitializeScreen();

	I2CMultiplexer::init();
	NeoPixelController::init();
#if defined(USE_SSD1306_DRIVER_ARRAY)
	DisplayController::initScreenArray();
#endif
	VisualRenderer::init();
	VisualManager::init();
	ButtonController::init();
	WheelController::init();
	KeyboardController::init();
	RangingController::init();

	delay(100);
	if(ButtonController::scan(micros()) & CONFIGURE_BUTTON) {
        processor = Processor::createProcessor(Configure);
	} else {
        processor = Processor::createProcessor(KeyboardMouse);
		if(processor == nullptr) {
			processor = Processor::createProcessor(Configure);
		}
	}
}

void loop() {
	processor->process(micros());
}

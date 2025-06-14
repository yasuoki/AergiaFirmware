//
// Created by yasuoki on 24/12/23.
//

#include "wheel_controller.h"
#include "display_controller.h"

WheelController::State WheelController::state=WheelController::STOP;
uint32_t WheelController::lastATime = 0;
uint32_t WheelController::lastBTime = 0;
WheelController::Fase  WheelController::lastFase = FASE_NONE;
int8_t WheelController::wheelAVal = 0;
int8_t WheelController::wheelBVal = 0;
WheelController::Fase WheelController::triggerFase = FASE_NONE;
int8_t WheelController::triggerValue = 0;
uint32_t minInterval = 5000UL;
uint32_t maxInterval = 100000UL;

void WheelController::init()
{
    pinMode(PIN_WHEEL_A, INPUT);
    pinMode(PIN_WHEEL_B, INPUT);
}

void WheelController::scanTrigger(int8_t a, int8_t b)
{
	if (a != wheelAVal) {
		state = TRIGGER;
		triggerFase = FASE_A;
		triggerValue = a;
		return;
	}
	if (b != wheelBVal) {
		state = TRIGGER;
		triggerFase = FASE_B;
		triggerValue = b;
		return;
	}
}

int8_t WheelController::scanFirst(int8_t a, int8_t b)
{
    int8_t scanValue = 0;
    if(triggerFase == FASE_A) {
        if(a != triggerValue) {
            state = STOP;
			return 0;
        }
		if (a == HIGH) {
			if (b == HIGH) {
				state = FORWARD;
				scanValue = 1;
			} else {
				state = BACKWARD;
				scanValue = -1;
			}
		} else {
			if (b == HIGH) {
				state = BACKWARD;
				scanValue = -1;
			} else {
				state = FORWARD;
				scanValue = 1;
			}
		}
    } else {
        if(b != triggerValue) {
            state = STOP;
			return 0;
        }
		if (b == HIGH) {
			if (a == HIGH) {
				state = BACKWARD;
				scanValue = -1;
			} else {
				state = FORWARD;
				scanValue = 1;
			}
		} else {
			if (a == HIGH) {
				state = FORWARD;
				scanValue = 1;
			} else {
				state = BACKWARD;
				scanValue = -1;
			}
		}
    }
    return scanValue;
}
int8_t WheelController::scanWheel(int8_t a, int8_t b)
{
	int8_t wheelLastVal=0;
	if(a != wheelAVal) {
		if (a == HIGH) {
			if (b == HIGH) {
				wheelLastVal = 1;
			} else {
				wheelLastVal = -1;
			}
		} else {
			if (b == HIGH) {
				wheelLastVal = -1;
			} else {
				wheelLastVal = 1;
			}
		}
	} else if(b != wheelBVal) {
		if (b == HIGH) {
			if (a == HIGH) {
				wheelLastVal = -1;
			} else {
				wheelLastVal = 1;
			}
		} else {
			if (a == HIGH) {
				wheelLastVal = 1;
			} else {
				wheelLastVal = -1;
			}
		}
	}
	return wheelLastVal;

}

int8_t WheelController::scanForward(int8_t a, int8_t b)
{
    int8_t scanValue = 0;
    if(triggerFase == FASE_A) {
		if(a != wheelAVal) {
			scanValue = scanWheel(a, b);
			if (scanValue < 0) {
				scanValue = 0;
				state = STOP;
			}
		}
    } else {
		if(b != wheelBVal) {
			scanValue = scanWheel(a, b);
			if (scanValue < 0) {
				scanValue = 0;
				state = STOP;
			}
		}
    }
    return scanValue;
}

int8_t WheelController::scanBackward(int8_t a, int8_t b)
{
    int8_t scanValue = 0;
	if(triggerFase == FASE_A) {
		if(a != wheelAVal) {
			scanValue = scanWheel(a, b);
			if (scanValue > 0) {
				scanValue = 0;
				state = STOP;
			}
		}
	} else {
		if(b != wheelBVal) {
			scanValue = scanWheel(a, b);
			if (scanValue > 0) {
				scanValue = 0;
				state = STOP;
			}
		}
	}
    return scanValue;
}

int8_t WheelController::scan(uint32_t now)
{
    int8_t a = (int8_t)digitalRead(PIN_WHEEL_A);
    int8_t b = (int8_t)digitalRead(PIN_WHEEL_B);
/*
	ScreenGFX* screen = DisplayController::getScreen(0);
	if(a == HIGH) {
		screen->gfx->fillRect(100,110,20,20,DisplayController::rgb565Color(0x00,0x00,0xff));
	} else {
		screen->gfx->fillRect(100,110,20,20,DisplayController::rgb565Color(0x00,0x00,0x00));
	}
	if(b == HIGH) {
		screen->gfx->fillRect(140,110,20,20,DisplayController::rgb565Color(0xff,0x00,0x00));
	} else {
		screen->gfx->fillRect(140,110,20,20,DisplayController::rgb565Color(0x00,0x00,0x00));
	}
 */
	bool lp = false;
	if( a != wheelAVal || b != wheelBVal ) {
		if(a != wheelAVal) {
			if(lastFase == FASE_A || (now - lastATime < minInterval))
				lp = true;
		}
		if(b != wheelBVal) {
			if(lastFase == FASE_B || (now - lastBTime < minInterval))
				lp = true;
		}
	} else if((now - lastATime > maxInterval) || (now - lastBTime > maxInterval)) {
		lp = true;
	}

	int8_t scanValue = 0;//scanWheel(now,a,b);
	if(!lp) {
		switch (state) {
			case STOP:
				scanTrigger(a, b);
				break;
			case TRIGGER:
				scanValue = scanFirst(a, b);
				break;
			case FORWARD:
				scanValue = scanForward(a, b);
				break;
			case BACKWARD:
				scanValue = scanBackward(a, b);
				break;
		}
	}
    if(wheelAVal != a) {
		lastFase = FASE_A;
        wheelAVal = a;
        lastATime = now;
    }
	if(wheelBVal != b) {
		lastFase = FASE_B;
		wheelBVal = b;
		lastBTime = now;
	}
    return scanValue;
}

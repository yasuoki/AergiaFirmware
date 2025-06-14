//
// Created by Yasuoki on 2025/03/18.
//

/*
R LHH
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000004; // 3
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x000001; // 1
		if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x020000; // 18 ***
R HLH
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000040; // 7
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x000010; // 5
		if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x000200; // 10
R HHL
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000800; // 12
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x004000; // 15
		if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x001000; // 13
C LHH
		if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00000008; // 4
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000080; // 8
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00000400; // 11
C HLH
		if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00000002; // 2
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000020; // 6
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00008000; // 16
C HHL
		if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00010000; // 17
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000100; // 9
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00002000; // 14

#if defined(ENABLE_KEYBOARD_STABLE_SCAN)
R HLL
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000008; // 4
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x000002; // 2
		if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x010000; // 17
R LHL
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000080; // 8
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x000020; // 6
		if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x000100; // 9
R LLH
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000400; // 11
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x008000; // 16
		if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x002000; // 14
C HLL
		if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00000004; // 3
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000040; // 7
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00000800; // 12
C LHL
		if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00000001; // 1
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000010; // 5
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00004000; // 15
C LLH
		if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00020000; // 18
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000200; // 10
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00001000; // 13

 */
#include "keyboard_controller.h"

int KeyboardController::_scanFase = 0;
uint32_t KeyboardController::_lastCode = 0;
uint32_t KeyboardController::_lastTime = 0;
uint32_t KeyboardController::_makeCode = 0;

void KeyboardController::init() {
#ifdef PIN_BUTTON_0
	pinMode(PIN_BUTTON_0, INPUT_PULLUP);
#endif
#ifdef PIN_BUTTON_1
	pinMode(PIN_BUTTON_1, INPUT_PULLUP);
#endif
}

uint32_t KeyboardController::scan(uint32_t now) {
	if(_scanFase == 0) {
		if (now - _lastTime <= 20000) {
			return _lastCode;
		}
	}
	_scanImpl(now);
	return _lastCode;
}

void KeyboardController::_scanImpl(uint32_t now)
{
	if(_scanFase == 0) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		_makeCode = 0;
		pinMode(PIN_KEYBOARD_COL_1, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_COL_2, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_COL_3, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_ROW_1, OUTPUT);
		pinMode(PIN_KEYBOARD_ROW_2, OUTPUT);
		pinMode(PIN_KEYBOARD_ROW_3, OUTPUT);
		digitalWrite(PIN_KEYBOARD_ROW_1, LOW);
		digitalWrite(PIN_KEYBOARD_ROW_2, HIGH);
		digitalWrite(PIN_KEYBOARD_ROW_3, HIGH);
		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 1) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000004; // 3
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x000001; // 1
		// if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x020000; // 18 H/W not impl
		digitalWrite(PIN_KEYBOARD_ROW_1, HIGH);
		digitalWrite(PIN_KEYBOARD_ROW_2, LOW);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 2) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000040; // 7
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x000010; // 5
		if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x000200; // 10
		digitalWrite(PIN_KEYBOARD_ROW_2, HIGH);
		digitalWrite(PIN_KEYBOARD_ROW_3, LOW);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 3) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == LOW) _makeCode |= 0x000800; // 12
		if(digitalRead(PIN_KEYBOARD_COL_2) == LOW) _makeCode |= 0x004000; // 15
		if(digitalRead(PIN_KEYBOARD_COL_3) == LOW) _makeCode |= 0x001000; // 13

		pinMode(PIN_KEYBOARD_ROW_1, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_ROW_2, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_ROW_3, INPUT_PULLUP);
		pinMode(PIN_KEYBOARD_COL_1, OUTPUT);
		pinMode(PIN_KEYBOARD_COL_2, OUTPUT);
		pinMode(PIN_KEYBOARD_COL_3, OUTPUT);
		digitalWrite(PIN_KEYBOARD_COL_1, LOW);
		digitalWrite(PIN_KEYBOARD_COL_2, HIGH);
		digitalWrite(PIN_KEYBOARD_COL_3, HIGH);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 4) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00000008; // 4
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000080; // 8
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00000400; // 11
		digitalWrite(PIN_KEYBOARD_COL_1, HIGH);
		digitalWrite(PIN_KEYBOARD_COL_2, LOW);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 5) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00000002; // 2
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000020; // 6
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00008000; // 16
		digitalWrite(PIN_KEYBOARD_COL_2, HIGH);
		digitalWrite(PIN_KEYBOARD_COL_3, LOW);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 6) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		// if(digitalRead(PIN_KEYBOARD_ROW_1) == LOW) _makeCode |= 0x00010000; // 17 H/W not impl
		if(digitalRead(PIN_KEYBOARD_ROW_2) == LOW) _makeCode |= 0x00000100; // 9
		if(digitalRead(PIN_KEYBOARD_ROW_3) == LOW) _makeCode |= 0x00002000; // 14

#if defined(ENABLE_KEYBOARD_STABLE_SCAN)
		pinMode(PIN_KEYBOARD_ROW_1, OUTPUT);
		pinMode(PIN_KEYBOARD_ROW_2, OUTPUT);
		pinMode(PIN_KEYBOARD_ROW_3, OUTPUT);
		pinMode(PIN_KEYBOARD_COL_1, INPUT_PULLDOWN);
		pinMode(PIN_KEYBOARD_COL_2, INPUT_PULLDOWN);
		pinMode(PIN_KEYBOARD_COL_3, INPUT_PULLDOWN);
		digitalWrite(PIN_KEYBOARD_ROW_1, HIGH);
		digitalWrite(PIN_KEYBOARD_ROW_2, LOW);
		digitalWrite(PIN_KEYBOARD_ROW_3, LOW);

		_lastTime = now;
		_scanFase++;
		return;
#else
		_lastCode = _makeCode;
		_lastTime = now;
		_scanFase = 0;
#endif
	}

#if defined(ENABLE_KEYBOARD_STABLE_SCAN)
	if(_scanFase == 7) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000008; // 4
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x000002; // 2
		// if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x010000; // 17 H/W not impl
		digitalWrite(PIN_KEYBOARD_ROW_1, LOW);
		digitalWrite(PIN_KEYBOARD_ROW_2, HIGH);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 8) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000080; // 8
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x000020; // 6
		if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x000100; // 9
		digitalWrite(PIN_KEYBOARD_ROW_2, LOW);
		digitalWrite(PIN_KEYBOARD_ROW_3, HIGH);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 9) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_COL_1) == HIGH) _makeCode |= 0x000400; // 11
		if(digitalRead(PIN_KEYBOARD_COL_2) == HIGH) _makeCode |= 0x008000; // 16
		if(digitalRead(PIN_KEYBOARD_COL_3) == HIGH) _makeCode |= 0x002000; // 14

		pinMode(PIN_KEYBOARD_COL_1, OUTPUT);
		pinMode(PIN_KEYBOARD_COL_2, OUTPUT);
		pinMode(PIN_KEYBOARD_COL_3, OUTPUT);
		pinMode(PIN_KEYBOARD_ROW_1, INPUT_PULLDOWN);
		pinMode(PIN_KEYBOARD_ROW_2, INPUT_PULLDOWN);
		pinMode(PIN_KEYBOARD_ROW_3, INPUT_PULLDOWN);
		digitalWrite(PIN_KEYBOARD_COL_1, HIGH);
		digitalWrite(PIN_KEYBOARD_COL_2, LOW);
		digitalWrite(PIN_KEYBOARD_COL_3, LOW);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 10) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00000004; // 3
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000040; // 7
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00000800; // 12
		digitalWrite(PIN_KEYBOARD_COL_1, LOW);
		digitalWrite(PIN_KEYBOARD_COL_2, HIGH);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 11) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00000001; // 1
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000010; // 5
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00004000; // 15
		digitalWrite(PIN_KEYBOARD_COL_2, LOW);
		digitalWrite(PIN_KEYBOARD_COL_3, HIGH);

		_lastTime = now;
		_scanFase++;
		return;
	}
	if(_scanFase == 12) {
		if(now - _lastTime < KEYBOARD_BUS_SETTLE_DELAY)
			return;
		// if(digitalRead(PIN_KEYBOARD_ROW_1) == HIGH) _makeCode |= 0x00020000; // 18 H/W not impl
		if(digitalRead(PIN_KEYBOARD_ROW_2) == HIGH) _makeCode |= 0x00000200; // 10
		if(digitalRead(PIN_KEYBOARD_ROW_3) == HIGH) _makeCode |= 0x00001000; // 13
		_lastCode = _makeCode;
		_lastTime = now;
		_scanFase = 0;
		return;
	}
#endif // ENABLE_KEYBOARD_STABLE_SCAN
}


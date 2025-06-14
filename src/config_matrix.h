//
// Created by Yasuoki on 2025/03/18.
//
#ifndef INC_AERGIA_CONFIG_MATRIX_H
#define INC_AERGIA_CONFIG_MATRIX_H

#include "config_matrix_board.h"

#define USE_SSD1306_DRIVER_ARRAY
#define PIN_SSD1306_RESET 	-1
#define SSD1306_ADDRESS 	0x3c
#define SSD1306_WIDTH 		128
#define SSD1306_HEIGHT 		64
#define SSD1306_OFFSET_X 	0
#define SSD1306_OFFSET_Y 	16
#define SSD1306_ARRAY_COUNT	16
#define SSD1306_MAP 		{ \
	/*  0 */ ControlId::KeySwitch3, \
	/*  1 */ ControlId::KeySwitch7, \
	/*  2 */ ControlId::KeySwitch11, \
	/*  3 */ ControlId::KeySwitch15, \
	/*  4 */ ControlId::KeySwitch6, \
	/*  5 */ ControlId::KeySwitch2, \
	/*  6 */ ControlId::KeySwitch10, \
	/*  7 */ ControlId::KeySwitch14, \
	/*  8 */ ControlId::KeySwitch1, \
	/*  9 */ ControlId::KeySwitch5, \
	/* 10 */ ControlId::KeySwitch9, \
	/* 11 */ ControlId::KeySwitch13, \
	/* 12 */ ControlId::KeySwitch4, \
	/* 13 */ ControlId::KeySwitch0, \
	/* 14 */ ControlId::KeySwitch8, \
	/* 15 */ ControlId::KeySwitch12 \
	}

#endif //INC_AERGIA_CONFIG_MATRIX_H

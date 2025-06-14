//
// Created by Yasuoki on 2025/05/21.
//

#ifndef INC_AERGIA_CONFIG_MATRIX_S_H
#define INC_AERGIA_CONFIG_MATRIX_S_H

#if defined(USE_SSD1306_DRIVER_ARRAY)
#undef USE_SSD1306_DRIVER_ARRAY
#endif

#include "config_matrix_board.h"
#define SUBFRAME_WIDTH 		48
#define SUBFRAME_HEIGHT 	24
#define SUBFRAME_COUNT		16
#define SUBFRAME_MAP { \
	/*  0 */ ControlId::KeySwitch12, \
	/*  1 */ ControlId::KeySwitch13, \
	/*  2 */ ControlId::KeySwitch14, \
	/*  3 */ ControlId::KeySwitch15, \
	/*  4 */ ControlId::KeySwitch8, \
	/*  5 */ ControlId::KeySwitch9, \
	/*  6 */ ControlId::KeySwitch10, \
	/*  7 */ ControlId::KeySwitch11, \
	/*  8 */ ControlId::KeySwitch4, \
	/*  9 */ ControlId::KeySwitch5, \
	/* 10 */ ControlId::KeySwitch6, \
	/* 11 */ ControlId::KeySwitch7, \
	/* 12 */ ControlId::KeySwitch0, \
	/* 13 */ ControlId::KeySwitch1, \
	/* 14 */ ControlId::KeySwitch2, \
	/* 15 */ ControlId::KeySwitch3, \
	}

#endif //INC_AERGIA_CONFIG_MATRIX_S_H

//
// Created by Yasuoki on 2024/12/06.
//

#ifndef INC_AERGIA_CONFIG_H
#define INC_AERGIA_CONFIG_H

#define VERSION "1.0.0"

#define DEVICE_TYPE		"Aergia"

#include "aergia_types.h"

#if defined(FALCON_DEVICE)
#include "config_falcon.h"
#define DEVICE_MODEL "Aergia Falcon"
#define MODEL "Falcon"
#elif defined(MATRIX_DEVICE)
#include "config_matrix.h"
#define DEVICE_MODEL "Aergia Matrix"
#define MODEL "Matrix"
#elif defined(MATRIX_S_DEVICE)
#include "config_matrix_s.h"
#define DEVICE_MODEL "Aergia Matrix S"
#define MODEL "Matrix S"
#else
#error	"device type is not defined"
#endif

#define KEYBOARD_BUS_SETTLE_DELAY		100L
#define ENABLE_KEYBOARD_STABLE_SCAN

#define BUTTON_STATE_0	0x01
#define BUTTON_STATE_1	0x02
#define BUTTON_STATE_2	0x04
#define BUTTON_STATE_3	0x08
#define BUTTON_STATE_4	0x10
#define BUTTON_STATE_5	0x20
#define BUTTON_STATE_6	0x40
#define BUTTON_STATE_7	0x80

#endif //INC_AERGIA_CONFIG_H

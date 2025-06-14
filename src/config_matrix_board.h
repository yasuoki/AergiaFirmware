//
// Created by Yasuoki on 2025/06/03.
//
// RP2040 Zero PINS
//                  5V          00  I2C0 SDA
//                  GND         01  I2C0 SCL
//                  3.3V        02  J/S WHEEL A
//  KB Modifier1    29          03  J/S WHEEL B
//  KB Modifier2    28          04  SPI0 RX
//  LCD BLK         27          05  SPI0 CS
//  LCB DC          26          06  SPI0 SCK
//  LCD RES         15          07  SPI0 TX
//   Keyboard Row 1 14          08 Keyboard Column 2
//                     1 1 1 1 0
//                     3 2 1 0 9
//                     | | | | +-- Keyboard Row 3
//                     | | | +---- NeoPixel
//                     | | +------ Keyboard Column 1
//                     | +-------- Keyboard Row 2
//                     +---------- Keyboard Column 3

#ifndef INC_AERGIA_CONFIG_MATRIX_BOARD_H
#define INC_AERGIA_CONFIG_MATRIX_BOARD_H

#include "aergia_types.h"

#define PIN_I2C_SDA			0
#define PIN_I2C_SCL			1

// I2C multiplexer configuration
#define IIC_MUX_CHANNELS	2
#define IIC_MUX_CH0_ADDRESS	0x70
#define IIC_MUX_CH1_ADDRESS	0x71

// NeoPixel LED Controller
#define PIN_NEO_PIXEL_LED	10
#define NEO_PIXEL_LED_COUNT 18
#define NEO_PIXEL_LED_MAP  { \
    /*  0 */ ControlId::KeySwitch15, \
    /*  1 */ ControlId::KeySwitch14, \
    /*  2 */ ControlId::KeySwitch13, \
    /*  3 */ ControlId::KeySwitch12, \
    /*  4 */ ControlId::KeySwitch8, \
    /*  5 */ ControlId::KeySwitch9, \
    /*  6 */ ControlId::KeySwitch10, \
    /*  7 */ ControlId::KeySwitch11, \
    /*  8 */ ControlId::KeySwitch7, \
    /*  9 */ ControlId::KeySwitch6, \
    /* 10 */ ControlId::KeySwitch5, \
    /* 11 */ ControlId::KeySwitch4, \
    /* 12 */ ControlId::KeySwitch0, \
    /* 13 */ ControlId::KeySwitch1, \
    /* 14 */ ControlId::KeySwitch2, \
    /* 15 */ ControlId::KeySwitch3, \
    /* 16 */ ControlId::Button0, \
    /* 17 */ ControlId::Button1  \
    }

// Display controller configuration
#define PIN_ST7789_RX		4	// NC
#define PIN_ST7789_CS		5	// CS
#define PIN_ST7789_SCK		6	// SCL
#define PIN_ST7789_TX		7	// SDA
#define PIN_ST7789_DC		26	// DC
#define PIN_ST7789_BLK		27	// BLK
#define PIN_ST7789_RESET	15	// RES
#define ST7789_WIDTH 		240
#define ST7789_HEIGHT 		240 /*198*/

// Button Controller configuration
#define BUTTON_COUNT    2
#define PIN_BUTTON_0	29
#define PIN_BUTTON_1	28
#define CONFIGURE_BUTTON (BUTTON_STATE_0|BUTTON_STATE_1)

// KwySwitch controller configuration
#define KEYBOARD_KEY_COUNT	16
#define PIN_KEYBOARD_ROW_1  14
#define PIN_KEYBOARD_ROW_2  12
#define PIN_KEYBOARD_ROW_3  9
#define PIN_KEYBOARD_COL_1  11
#define PIN_KEYBOARD_COL_2  8
#define PIN_KEYBOARD_COL_3  13

// Wheel controller configuration
#define PIN_WHEEL_A			2
#define PIN_WHEEL_B			3

// Ranging device configuration
#define RANGING_DEVICE_ADDRESS	0x29
#define RANGE_SCAN_INTERVAL		200000UL
#define RANGE_THRESHOLD			70

#endif //INC_AERGIA_CONFIG_MATRIX_BOARD_H

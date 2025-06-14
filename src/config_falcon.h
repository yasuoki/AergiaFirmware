//
// Created by Yasuoki on 2025/04/14.
//

#ifndef CONFIG_FALCON_H
#define CONFIG_FALCON_H

// RP2040 Zero PINS
//                  5V          00  I2C0 SDA
//                  GND         01  I2C0 SCL
//                  3.3V        02  J/S WHEEL A
//  AUX             29          03  J/S WHEEL B
//  AUX             28          04  SPI0 RX
//  LCD BLK         27          05  SPI0 CS
//  LCB DC          26          06  SPI0 SCK
//  LED             15          07  SPI0 TX
//   Keyboard Row 1 14          08 Keyboard Column 2
//                     1 1 1 1 0
//                     3 2 1 0 9
//                     | | | | +-- Keyboard Row 3
//                     | | | +---- NeoPixel
//                     | | +------ Keyboard Column 1
//                     | +-------- Keyboard Row 2
//                     +---------- Keyboard Column 3

// ESP32 S3 Zero PINS
//            5V           TX
//           GND           RX
//          3.3V           13  I2C SDA
//  K/B R1     1           12  I2C SCL
//  K/B R2     2           11  J/S WHEEL A
//  K/B C1     3           10  J/S WHEEL B
//  K/B C2     4           9   SPI MOSI/SDA
//  LED        5           8   SPI CLK/SCL
//  J/S SHIFT  6           7   SPI DC


#endif //CONFIG_FALCON_H

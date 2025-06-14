//
// Created by Yasuoki on 2025/05/14.
//

#include "config.h"
#include "i2c_multiplexer.h"
#include <Wire.h>

int I2CMultiplexer::curChannelAddr = -1;
int I2CMultiplexer::curChannel = -1;
int I2CMultiplexer::curBus = -1;

void I2CMultiplexer::init() {
#if IIC_MUX_CHANNELS > 0
	Wire.beginTransmission(IIC_MUX_CH0_ADDRESS);
	Wire.write(0);
	Wire.endTransmission();
#endif
#if IIC_MUX_CHANNELS > 1
	Wire.beginTransmission(IIC_MUX_CH1_ADDRESS);
	Wire.write(0);
	Wire.endTransmission();
#endif
}
void I2CMultiplexer::select(int bus)
{
	if(curBus != bus) {
		int ch = bus / 8;
		int busAddr = bus % 8;
		if(curChannel != ch && curChannel != -1) {
			Wire.beginTransmission(curChannelAddr);
			Wire.write(0);
			Wire.endTransmission();
		}
#if IIC_MUX_CHANNELS > 0
		if(ch == 0) {
			Wire.beginTransmission(IIC_MUX_CH0_ADDRESS);
			Wire.write(1<<busAddr);
			Wire.endTransmission();
			curChannelAddr = IIC_MUX_CH0_ADDRESS;
			curChannel = ch;
			curBus = bus;
		}
#endif
#if IIC_MUX_CHANNELS > 1
		if(ch == 1) {
			Wire.beginTransmission(IIC_MUX_CH1_ADDRESS);
			Wire.write(1<<busAddr);
			Wire.endTransmission();
			curChannelAddr = IIC_MUX_CH1_ADDRESS;
			curChannel = ch;
			curBus = bus;
		}
#endif
	}
}


//
// Created by Yasuoki on 2025/05/14.
//

#ifndef INC_AERGIA_I2C_MULTIPLEXER_H
#define INC_AERGIA_I2C_MULTIPLEXER_H

#include "config.h"

class I2CMultiplexer {
private:
	static int curChannelAddr;
	static int curChannel;
	static int curBus;

public:
	static void init();
	static void select(int bus);
};

#endif //INC_AERGIA_I2C_MULTIPLEXER_H

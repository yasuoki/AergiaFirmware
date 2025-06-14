//
// Created by Yasuoki on 2025/05/14.
//

#ifndef INC_AERGIA_CONFIG_LOADER_H
#define INC_AERGIA_CONFIG_LOADER_H

#include "config.h"

class Configuration {
public:
	ConfigData *root;

	Configuration(ConfigData *config) {
		this->root = config;
	}
};

class ConfigLoader final {
private:
	static uint8_t *basePtr;
	static void *offsetAddress(void *offset);
	ConfigLoader() {}
	~ConfigLoader() {}

	static ConfigData *linkPointers(uint8_t *buffer);
	static void linkVisual(Visual *visual);
	static void linkApplication(Application *application);
	static void linkPage(Page *page);
	static void linkControl(Control *control);
	static void linkActionCommand(ActionCommand *actionCommand);
	static void linkCommand(Command *command);
	static void linkMouseMoveCommand(Command *command);
	static void linkMouseWheelCommand(Command *command);
	static void linkButtonCommand(Command *command);
	static void linkKeyCommand(Command *command);
	static void linkMapInputCommand(Command *command);
	static void linkMapActionCommand(Command *command);
	static void linkPageChangeCommand(Command *command);
	static void linkApplicationChangeCommand(Command *command);
	static void linkDelayCommand(Command *command);
	static void linkTimerCommand(Command *command);
public:
	static void init();
	static Configuration *load(const char *file);
};


#endif //INC_AERGIA_CONFIG_LOADER_H

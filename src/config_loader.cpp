//
// Created by Yasuoki on 2025/05/14.
//
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include "config_loader.h"
#include "processor.h"
#include "control/button_controller.h"

uint8_t *ConfigLoader::basePtr = nullptr;

void ConfigLoader::init() {
	if(!LittleFS.begin()) {
		Processor::log("Filesystem mount error");
		while(ButtonController::scan(micros()) == 0) {
			delay(100);
		}
	}
}

Configuration *ConfigLoader::load(const char *file) {
	File fp = LittleFS.open(file, "r");
	if(!fp) {
		return nullptr;
	}
	size_t fileSize = fp.size();
	uint8_t *buf = (uint8_t *)malloc(fileSize);
	fp.read(buf, fileSize);
	fp.close();
	ConfigData *data = linkPointers(buf);
	return new Configuration(data);
}

void *ConfigLoader::offsetAddress(void* offset)
{
	if((uint8_t*)offset < basePtr)
		return basePtr + (uint)offset;
	return offset;
}

ConfigData *ConfigLoader::linkPointers(uint8_t *buffer)
{
	basePtr = buffer;
	ConfigData *data = (ConfigData*)buffer;
	for(int i = 0; i < data->count; i++) {
		data->appRef[i].appNamePtr = (char *)offsetAddress(data->appRef[i].appNamePtr);
		data->appRef[i].applicationPtr = (Application *)offsetAddress(data->appRef[i].applicationPtr);
		linkApplication(data->appRef[i].applicationPtr);
	}
	return data;
}

void ConfigLoader::linkVisual(Visual *visual) {
	if(visual->text != nullptr)
		visual->text = (char *)offsetAddress(visual->text);
}

void ConfigLoader::linkApplication(Application *application)
{
	if(application->visualPtr != nullptr) {
		application->visualPtr = (Visual *) offsetAddress(application->visualPtr);
		linkVisual(application->visualPtr);
	}
	for(int i = 0; i < application->count; i++) {
		application->pages[i].pageNamePtr = (char *)offsetAddress(application->pages[i].pageNamePtr);
		application->pages[i].pagePtr = (Page *)offsetAddress(application->pages[i].pagePtr);
		linkPage(application->pages[i].pagePtr);
	}
}

void ConfigLoader::linkPage(Page *page)
{
	if(page->visualPtr != nullptr) {
		page->visualPtr = (Visual *) offsetAddress(page->visualPtr);
		linkVisual(page->visualPtr);
	}
	for(int i = 0; i < page->count; i++) {
		page->controlBind[i].controlPtr = (Control *)offsetAddress(page->controlBind[i].controlPtr);
		linkControl(page->controlBind[i].controlPtr);
	}
}

void ConfigLoader::linkControl(Control *control)
{
	if(control->visualPtr != nullptr) {
		control->visualPtr = (Visual *) offsetAddress(control->visualPtr);
		linkVisual(control->visualPtr);
	}
	for(int i = 0; i < control->count; i++) {
		control->eventBind[i].actionCommandPtr = (ActionCommand *)offsetAddress(control->eventBind[i].actionCommandPtr);
		linkActionCommand(control->eventBind[i].actionCommandPtr);
	}
}

void ConfigLoader::linkActionCommand(ActionCommand *actionCommand)
{
	for(int i = 0; i < actionCommand->count; i++) {
		actionCommand->commandPtr[i] = (Command*)offsetAddress(actionCommand->commandPtr[i]);
		linkCommand(actionCommand->commandPtr[i]);
	}
}

void ConfigLoader::linkCommand(Command *command)
{
	switch(command->commandId) {
		case MouseMove:
			linkMouseMoveCommand(command);
			break;
		case MouseTrackingStart:
			break;
		case MouseTrackingStop:
			break;
		case MouseTrackingRewind:
			break;
		case MouseWheel:
			linkMouseWheelCommand(command);
			break;
		case MouseClick:
		case MouseDoubleClick:
		case MouseTripleClick:
		case ButtonPress:
		case ButtonRelease:
			linkButtonCommand(command);
			break;
		case KeyPress:
		case KeyRelease:
		case KeyInput:
			linkKeyCommand(command);
			break;
		case MapInput:
			linkMapInputCommand(command);
			break;
		case MapAction:
			linkMapActionCommand(command);
			break;
		case PageChange:
			linkPageChangeCommand(command);
			break;
		case ApplicationChange:
			linkApplicationChangeCommand(command);
			break;
		case Delay:
			linkDelayCommand(command);
			break;
		case SetTimer:
			linkTimerCommand(command);
			break;
	}
}

void ConfigLoader::linkMouseMoveCommand(Command *command)
{
}
void ConfigLoader::linkMouseWheelCommand(Command *command)
{
}
void ConfigLoader::linkButtonCommand(Command *command)
{
}
void ConfigLoader::linkKeyCommand(Command *command)
{
	command->param.keyCommand.usageListPtr = (UsageList *)offsetAddress(command->param.keyCommand.usageListPtr);
}
void ConfigLoader::linkMapInputCommand(Command *command)
{
	for(int i = 0; i < command->param.mapInputCommand.count; i++) {
		command->param.mapInputCommand.mapData[i].usageListPtr = (UsageList *)offsetAddress(command->param.mapInputCommand.mapData[i].usageListPtr);
	}
}
void ConfigLoader::linkMapActionCommand(Command *command)
{
	for(int i = 0; i < command->param.mapActionCommand.count; i++) {
		command->param.mapActionCommand.mapData[i].actionCommandPtr = (ActionCommand *)offsetAddress(command->param.mapActionCommand.mapData[i].actionCommandPtr);
		linkActionCommand(command->param.mapActionCommand.mapData[i].actionCommandPtr);
	}
}
void ConfigLoader::linkPageChangeCommand(Command *command)
{
	command->param.pageChangeCommand.pageNamePtr = (char *)offsetAddress(command->param.pageChangeCommand.pageNamePtr);
}
void ConfigLoader::linkApplicationChangeCommand(Command *command)
{
	command->param.applicationChangeCommand.applicationNamePtr = (char *)offsetAddress(command->param.applicationChangeCommand.applicationNamePtr);
	if(command->param.applicationChangeCommand.pageNamePtr != nullptr)
		command->param.applicationChangeCommand.pageNamePtr = (char *)offsetAddress(command->param.applicationChangeCommand.pageNamePtr);
}
void ConfigLoader::linkDelayCommand(Command *command)
{
}
void ConfigLoader::linkTimerCommand(Command *command)
{
}

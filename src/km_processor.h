//
// Created by yasuoki on 25/01/30.
//

#ifndef INC_AERGIA_KM_PROCESSOR_H
#define INC_AERGIA_KM_PROCESSOR_H

#include "config.h"
#include "processor.h"
#include "config_loader.h"

#define TIMER_SOURCE_COUNT	24

typedef struct _TimerData {
	uint32_t	timeout;
	int16_t 	data;
} TimerData;

class KMProcessor : public Processor {
private:
	static Configuration *	_config;
	static uint16_t			_currentAppr;
	static uint16_t			_currentPage;
	static Application *	_currentApprPtr;
	static Page *			_currentPagePtr;
	static bool 			_needVisualApply;
	static bool 	_screenTimeout;
	static bool 	_mouseTracking;
	static int32_t 	_mouseTrackDistanceX;
	static int32_t 	_mouseTrackDistanceY;
	static uint32_t	_nextTimeout;
	static TimerData _timerData[TIMER_SOURCE_COUNT];

	void remapApplication();
	void remapPage();
	void changeApplication(uint16_t applicationIndex, uint16_t pageIndex);
	void nextApplication();
	void prevApplication();

	void changePage(uint16_t pageIndex);
	void nextPage();
	void prevPage();

	void doSleep();
	void doWakeup();
	void mouseMove(int32_t x, int32_t y);

	int intRefeerence(int v);
	float floatRefeerence(float v);
	bool compaireKey(float key, uint16_t op, float value);
	void clearAllTimers();

	void cmdMouseMove(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseTrackingStart(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseTrackingStop(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseTrackingRewind(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseWheel(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseClick(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseDoubleClick(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMouseTripleClick(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdButtonPress(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdButtonRelease(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdKeyPress(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdKeyRelease(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdKeyInput(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMapInput(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdMapAction(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdPageChange(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdApplicationChange(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdDelay(uint32_t now, ControlId control, EventId event, Command *command);
	void cmdSetTimer(uint32_t now, ControlId control, EventId event, Command *command);
	void doCommand(uint32_t now, ControlId control, EventId event, Command *command);

	void onTimer(ControlId from, uint32_t now);
	void eventDispatch(uint32_t now, ControlId control, EventId event);

public:
    KMProcessor();
    bool init() override;
	void onKeyDown(ControlId from, uint32_t now) override;
	void onKeyUp(ControlId from, uint32_t now) override;
	void onKeyInput(ControlId from, uint32_t now) override;
	void onKeyLongPress(ControlId from, uint32_t now) override;
    void onWheelBegin(ControlId from, uint32_t now, int8_t w) override;
    void onWheel(ControlId from, uint32_t now, int8_t w) override;
    void onWheelEnd(ControlId from, uint32_t now, int8_t w) override;
	void onEnter(ControlId from, uint32_t now) override;
	void onLeave(ControlId from, uint32_t now) override;
	void onUsbMount(ControlId from, uint32_t now) override;
	void onUsbUnmount(ControlId from, uint32_t now) override;
	void onLoop(uint32_t now, bool eventFired) override;

};

#endif //INC_AERGIA_KM_PROCESSOR_H

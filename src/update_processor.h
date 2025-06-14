//
// Created by yasuoki on 25/01/30.
//

#ifndef INC_AERGIA_UPDATE_PROCESSOR_H
#define INC_AERGIA_UPDATE_PROCESSOR_H

#include "processor.h"
#include <FS.h>
#include <LittleFS.h>

class UpdateProcessor: public Processor {
private:
	enum UpdateStatus {
		WaitConnection,
		Connected,
		Transfer,
		UpdateComplete,
	};
	UpdateStatus _status;
	uint32_t 	_lastLoop;
	int 		_messageCont;
	char 		_buffer[130];
	size_t		_bufferSize;
	char 		_uploadFileName[16];
	size_t		_uploadFileSize;
	File 		_file;
	size_t 		_receivedFileSize;
	size_t 		_receivedRatio;
	uint8_t 	_receiveBuffer[4096];
	size_t 		_receiveBufferSize;
	void waitConnectionProcess(uint32_t now, const uint8_t *data, size_t size);
	void connectedProcess(uint32_t now, const uint8_t *data, size_t size);
	void transferProcess(uint32_t now, const uint8_t *data, size_t size);
public:
	UpdateProcessor();
    bool virtual init();

	virtual void onSerialConnect(ControlId from, uint32_t now);
	virtual void onSerialDisconnect(ControlId from, uint32_t now);
	virtual void onSerial(ControlId from, uint32_t now, const uint8_t *data, size_t size);
	virtual void onLoop(uint32_t now);

};

#endif //INC_AERGIA_UPDATE_PROCESSOR_H

//
// Created by yasuoki on 25/01/30.
//
#include "update_processor.h"
#include <Adafruit_GFX.h>
#include <Adafruit_TinyUSB.h>
#include "visual_manager.h"
#include "visual_renderer.h"
#include "control/display_controller.h"

UpdateProcessor::UpdateProcessor() : _file(nullptr)
{
	_status = WaitConnection;
	_lastLoop = 0;
	_messageCont = 0;
	_buffer[0] = 0;
	_bufferSize = 0;
	_uploadFileName[0] = 0;
	_uploadFileSize = 0;
	_receivedFileSize = 0;
	_receivedRatio = 0;
	_receiveBufferSize = 0;
}

bool UpdateProcessor::init()
{
    ScreenGFX *screen = DisplayController::selectScreen(0);
	uint16_t orange = DisplayController::rgb565Color(0xff, 0xa5,0x00);
	screen->gfx->setTextColor(orange);
	screen->gfx->setTextSize(1);
    screen->gfx->fillScreen(0);
	screen->gfx->drawCircle(120,120,110,orange);
	VisualRenderer::setSmallFont(screen);
	VisualRenderer::drawText(screen, 68, "configuration");
	VisualRenderer::drawText(screen, 190, DEVICE_MODEL);
	VisualManager::hideLed();

	LittleFS.begin();

	TinyUSBDevice.setManufacturerDescriptor("Yonabe Factory");
	TinyUSBDevice.setProductDescriptor(DEVICE_MODEL);
	if (!TinyUSBDevice.isInitialized()) {
		TinyUSBDevice.begin(0);
	}
	if (TinyUSBDevice.mounted()) {
		TinyUSBDevice.detach();
		delay(10);
		TinyUSBDevice.attach();
		delay(10);
	}
	Serial.begin(115200);
	onSerialDisconnect(ControlId::Main,micros());
	return true;
}

void UpdateProcessor::onSerialConnect(ControlId from, uint32_t now)
{
}

void UpdateProcessor::onSerialDisconnect(ControlId from, uint32_t now)
{
	if(_status == UpdateComplete)
		return;

	if(_file) {
		_file.close();
	}
	_status = WaitConnection;
	_lastLoop = 0;
	_messageCont = 0;
	log("Waiting");
}

void UpdateProcessor::onSerial(ControlId from, uint32_t now, const uint8_t *data, size_t size)
{
	if(_status == UpdateComplete)
		return;
	if(_status == Transfer) {
		transferProcess(now, data, size);
		return;
	}
	char *ptr = &_buffer[_bufferSize];
	while(size > 0) {
		if(_bufferSize == sizeof(_buffer) - 1) {
			_bufferSize = 0;
			_status = WaitConnection;
			_lastLoop = 0;
			log("Buffer Full");
			return;
		}
		*ptr = (char)*data++; size--;
		if(*ptr == '\n') {
			*ptr = 0;
			if(_bufferSize > 1 && _buffer[_bufferSize - 1] == '\r') {
				*(ptr-1) = 0;
				_bufferSize--;
			}
			switch (_status) {
				case WaitConnection:
					waitConnectionProcess(now, (const uint8_t *) _buffer, _bufferSize);
					break;
				case Connected:
					connectedProcess(now, (const uint8_t *) _buffer, _bufferSize);
					break;
				case Transfer:
					transferProcess(now, (const uint8_t *) _buffer, _bufferSize);
					break;
			}
			ptr = _buffer;
			_bufferSize = 0;
		} else {
			ptr++;
			_bufferSize++;
		}
	}
}

void UpdateProcessor::waitConnectionProcess(uint32_t now, const uint8_t *data, size_t size)
{
	if(strcmp((char*)data, "connect") == 0) {
		_status = Connected;
		Serial.println("[Result] 00 OK, Connecting");
		log("Connected");
		return;
	}
	log((const char*)data);
	Serial.println("Yonabe Factory / "  DEVICE_MODEL  " / VERSION " VERSION);
	return;
}

void UpdateProcessor::connectedProcess(uint32_t now, const uint8_t *data, size_t size)
{
	if(strcmp((char*)data, "begin") == 0) {
		FSInfo fsinfo;
		LittleFS.format();
		LittleFS.info(fsinfo);
		long total = fsinfo.totalBytes / 1024;
		long used = fsinfo.usedBytes / 1024;
		uint blk = fsinfo.blockSize;
		Serial.printf("[Result] 00 OK, File system initialized blk%u %lu/%lu(kb)\r\n", blk, used, total);
		log("Initialized");;
		return;
	}
	if(strcmp((char*)data, "complete") == 0) {
		LittleFS.end();
		_status = UpdateComplete;
		log("Complete");
		Serial.printf("[Result] 00 OK, Rebooting in 2 seconds\r\n");
		watchdog_reboot(0,0,1000);
		return;
	}
	if(strncmp((char*)data, "upload ", 7) == 0) {
		const uint8_t *ptr = data + 7;
		size -= 7;
		size_t len = 0;
		char *dst = _uploadFileName;
		while(size > 0 && len < 16 && *ptr != '/') {
			*dst++ = *ptr++;
			len++;
		}
		if(*ptr != '/') {
			_status = WaitConnection;
			log("Invalid Data");
			return;
		}
		*dst = '\0';
		ptr++; size--; // skip delimiter '/'
		_uploadFileSize = 0;
		while(size > 0 && *ptr != '\0') {
			char c = *ptr++;
			if( c < '0' || '9' < c ) {
				_status = WaitConnection;
				log("Invalid Data");
				return;
			}
			_uploadFileSize = _uploadFileSize * 10 + (c - '0');
		}
		if(*ptr != '\0') {
			_status = WaitConnection;
			log("Invalid Data");
			return;
		}
		_file = LittleFS.open(_uploadFileName, "w");
		_receivedFileSize = 0;
		_receivedRatio = 0;
		_receiveBufferSize = 0;
		_status = Transfer;
		log(" ");
		Serial.printf("[Result] 00 OK, Start the upload. %s(%d)\r\n", _uploadFileName, _uploadFileSize);
		return;
	}
	_status = WaitConnection;
	log("Invalid Data");
}

void UpdateProcessor::transferProcess(uint32_t now, const uint8_t *data, size_t size)
{
	size_t remain = _uploadFileSize - _receivedFileSize;
	while(remain > 0 && size > 0) {
		size_t bufferSpace = sizeof(_receiveBuffer) - _receiveBufferSize;
		size_t writeSize = remain < bufferSpace ? remain : bufferSpace;
		if(writeSize > size)
			writeSize = size;
		size_t dataSize = 0;
		uint8_t *dst = &_receiveBuffer[_receiveBufferSize];
		while(writeSize > dataSize) {
			*dst++ = *data++;
			dataSize++;
		}
		_receiveBufferSize += dataSize;
		_receivedFileSize += dataSize;
		if(_receiveBufferSize == sizeof(_receiveBuffer)) {
			if(_file)
				_file.write(_receiveBuffer, _receiveBufferSize);
			_receiveBufferSize = 0;
		}
		remain -= dataSize;
		size -= dataSize;
	}
	if(remain == 0) {
		bool success = _file;
		if(_receiveBufferSize > 0) {
			if(_file)
				_file.write(_receiveBuffer, _receiveBufferSize);
		}
		if(_file)
			_file.close();
		_status = Connected;
		log(" ");
		if(success)
			Serial.printf("[Result] 00 OK, Upload complete. %s(%d)\r\n", _uploadFileName, _receivedFileSize);
		else
			Serial.printf("[Result] 10 File System Error\r\n");
	} else {
		int receiveRatio = (int)(_receivedFileSize * 10 / _uploadFileSize) + 1;
		if( receiveRatio > 10)
			receiveRatio = 10;
		if(_receivedRatio < receiveRatio) {
			VisualFrame *vf = VisualManager::getVisualFrame(ControlId::Display);
			ScreenGFX *screen = DisplayController::selectScreen(vf->screen);
			uint16_t orange = DisplayController::rgb565Color(0xff, 0xa5,0x00);
			for(int i = _receivedRatio; i < receiveRatio; i++) {
				screen->gfx->fillRect((int16_t)(20+20*i),120,16,16,orange);
			}
			_receivedRatio = receiveRatio;
		}
	}
}

void UpdateProcessor::onLoop(uint32_t now) {
	if (_status == WaitConnection) {
		if(now - _lastLoop > 1000000L) {
			if(_messageCont == 0) {
				log("Connection");
				_messageCont = 1;
			} else {
				log("Waiting");
				_messageCont = 0;
			}
			_lastLoop = now;
		}
	}
}

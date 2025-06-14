//
// Created by Yasuoki on 2025/05/27.
//

#ifndef INC_AERGIA_AERGIA_TYPES_H
#define INC_AERGIA_AERGIA_TYPES_H

#include <Arduino.h>
#include <cstdint>

enum ProcessorMode
{
	Configure,
	KeyboardMouse,
	MultiAxisJoyStick
};

enum PixelFormat
{
	Rgb888,
	Rgb565,
	BW1
};

enum MapComparator {
	Other = 0,
	Equal = 1,
	NotEqual = 2,
	Less = 3,
	LessOrEqual = 4,
	Greater = 5,
	GreaterOrEqual = 6,
};

enum CommandId {
	MouseMove = 0x8401,
	MouseTrackingStart = 0x8402,
	MouseTrackingStop  = 0x8403,
	MouseTrackingRewind  = 0x8404,
	MouseWheel  = 0x8405,
	MouseClick  = 0x8406,
	MouseDoubleClick  = 0x8407,
	MouseTripleClick  = 0x8408,
	ButtonPress  = 0x8409,
	ButtonRelease  = 0x840a,
	KeyPress  = 0x840b,
	KeyRelease  = 0x840c,
	KeyInput  = 0x840d,
	MapInput  = 0x840e,
	MapAction  = 0x840f,
	PageChange  = 0x8410,
	ApplicationChange  = 0x8411,
	Delay  = 0x8412,
	SetTimer  = 0x8413
};

enum ControlId
{
	Unknown = 0,
	KeySwitch0 = 0x8000,
	KeySwitch1 = 0x8001,
	KeySwitch2 = 0x8002,
	KeySwitch3 = 0x8003,
	KeySwitch4 = 0x8004,
	KeySwitch5 = 0x8005,
	KeySwitch6 = 0x8006,
	KeySwitch7 = 0x8007,
	KeySwitch8 = 0x8008,
	KeySwitch9 = 0x8009,
	KeySwitch10 = 0x800a,
	KeySwitch11 = 0x800b,
	KeySwitch12 = 0x800c,
	KeySwitch13 = 0x800d,
	KeySwitch14 = 0x800e,
	KeySwitch15 = 0x800f,
	Button0 = 0x8010,
	Button1 = 0x8011,
	Joystick = 0x8012,
	Wheel = 0x8013,
	Display = 0x8014,
	Ranging = 0x8015,
	Main = 0x8016,
	SerialPort = 0x8017
};
#define IS_BUTTON_ID(id) (ControlId::Button0 <= (id) && (id) <= ControlId::Button7)
#define IS_KEYSWITCH_ID(id) (ControlId::KeySwitch0 <= (id) && (id) <= ControlId::KeySwitch17)

enum EventId
{
//	UnNamed = 0x8200,
	KeyDownEvent 		= 0x8201,
	KeyUpEvent 			= 0x8202,
	KeyInputEvent 		= 0x8203,
	LongPressEvent		= 0x8204,
	BeginMoveEvent 		= 0x8205,
	MoveEvent 			= 0x8206,
	EndMoveEvent 		= 0x8207,
	BeginRotateEvent 	= 0x8208,
	RotateEvent 		= 0x8209,
	EndRotateEvent 		= 0x820a,
	BeginWheelEvent 	= 0x820b,
	WheelEvent 			= 0x820c,
	EndWheelEvent 		= 0x820d,
	EnterEvent 			= 0x820e,
	LeaveEvent 			= 0x820f,
	LoadEvent 			= 0x8210,
	ConnectEvent 		= 0x8211,
	DisconnectEvent 	= 0x8212,
	TimerEvent 			= 0x8213
};


enum VariableId {
	KeySwitch0_Status		= 0x8100,
	KeySwitch0_TimerData	= 0x8101,
	KeySwitch1_Status		= 0x8102,
	KeySwitch1_TimerData	= 0x8103,
	KeySwitch2_Status		= 0x8104,
	KeySwitch2_TimerData	= 0x8105,
	KeySwitch3_Status		= 0x8106,
	KeySwitch3_TimerData	= 0x8107,
	KeySwitch4_Status		= 0x8108,
	KeySwitch4_TimerData	= 0x8109,
	KeySwitch5_Status		= 0x810a,
	KeySwitch5_TimerData	= 0x810b,
	KeySwitch6_Status		= 0x810c,
	KeySwitch6_TimerData	= 0x810d,
	KeySwitch7_Status		= 0x810e,
	KeySwitch7_TimerData	= 0x810f,
	KeySwitch8_Status		= 0x8110,
	KeySwitch8_TimerData	= 0x8111,
	KeySwitch9_Status		= 0x8112,
	KeySwitch9_TimerData	= 0x8113,
	KeySwitch10_Status		= 0x8114,
	KeySwitch10_TimerData	= 0x8115,
	KeySwitch11_Status		= 0x8116,
	KeySwitch11_TimerData	= 0x8117,
	KeySwitch12_Status		= 0x8118,
	KeySwitch12_TimerData	= 0x8119,
	KeySwitch13_Status		= 0x811a,
	KeySwitch13_TimerData	= 0x811b,
	KeySwitch14_Status		= 0x811c,
	KeySwitch14_TimerData	= 0x811d,
	KeySwitch15_Status		= 0x811e,
	KeySwitch15_TimerData	= 0x811f,
	Button0_Status			= 0x8120,
	Button0_TimerData		= 0x8121,
	Button1_Status			= 0x8122,
	Button1_TimerData		= 0x8123,
	Joystick_X				= 0x8124,
	Joystick_Y				= 0x8125,
	Joystick_Z				= 0x8126,
	Joystick_MX				= 0x8127,
	Joystick_MY				= 0x8128,
	Joystick_MZ				= 0x8129,
	Joystick_Button			= 0x812a,
	Joystick_TimerData		= 0x812b,
	Wheel_Delta				= 0x812c,
	Wheel_TimerData			= 0x812d,
	Display_TimerData		= 0x812e,
	Ranging_Status			= 0x812f,
	Ranging_Distance		= 0x8130,
	Ranging_TimerData		= 0x8131,
	Main_TimerData			= 0x8132
};
#define IS_VARIABLE_ID(c) (VariableId::KeySwitch0_Status <= ((uint16_t)(c)) && ((uint16_t)(c)) <= VariableId::Main_Page)

#pragma pack(push, 1)

typedef struct _ActionCommand ActionCommand;

typedef struct _Visual {
	char * 	text;
	uint32_t color;
	uint32_t bgColor;
	uint16_t icon;
	uint16_t fontSize;
} Visual;

typedef struct _MouseMoveCommand {
	int16_t x;
	int16_t y;
	int16_t s;
	float	xr;
	float	yr;
} MouseMoveCommand;

typedef struct _MouseWheelCommand {
	int16_t delta;
	float	r;
} MouseWheelCommand;

typedef struct _ButtonCommand {
	uint16_t button;
} ButtonCommand;

typedef struct _UsageList {
	uint16_t 	len;
	uint8_t 	usage[1];
} UsageList;

typedef struct _KeyCommand {
	uint16_t	interval;
	UsageList *	usageListPtr;
} KeyCommand;

typedef struct _MapInputData {
	float		compData;
	UsageList *	usageListPtr;
	uint16_t 	op;
	uint16_t 	padding;
} MapInputData;

typedef struct MapInputCommand {
	uint16_t		count;
	float			key;
	MapInputData	mapData[1];
} MapInputCommand;

typedef struct _MapActionData {
	float		compData;
	ActionCommand *	actionCommandPtr;
	uint16_t 	op;
	uint16_t 	padding;
} MapActionData;

typedef struct _MapActionCommand {
	uint16_t		count;
	float	 		key;
	MapActionData	mapData[1];
} MapActionCommand;

typedef struct _PageChangeCommand {
	uint16_t		padding;
	char *       	pageNamePtr;
} PageChangeCommand;

typedef struct _ApplicationChangeCommand {
	uint16_t		padding;
	char *        	applicationNamePtr;
	char *	        pageNamePtr;
} ApplicationChangeCommand;

typedef struct _DelayCommand {
	uint16_t    timeout;
} DelayCommand;

typedef struct _TimerCommand {
	uint16_t    timeout;
	uint16_t    target;
	int16_t     data;
} TimerCommand;

typedef struct _Command {
	uint16_t 	commandId;
	union {
		MouseMoveCommand mouseMoveCommand;
		MouseWheelCommand mouseWheelCommand;
		ButtonCommand buttonCommand;
		KeyCommand keyCommand;
		MapInputCommand mapInputCommand;
		MapActionCommand mapActionCommand;
		PageChangeCommand pageChangeCommand;
		ApplicationChangeCommand applicationChangeCommand;
		DelayCommand delayCommand;
		TimerCommand timerCommand;
	} param;
} Command;

struct _ActionCommand {
	uint16_t 	count;
	uint16_t 	padding;
	Command *	commandPtr[1];
};

typedef struct _EventBind {
	uint16_t	eventId;
	uint16_t 	padding;
	ActionCommand *	actionCommandPtr;
} EventBind;

typedef struct _Control {
	Visual *	visualPtr;
	uint16_t	count;
	uint16_t	padding;
	EventBind	eventBind[1];
} Control;

typedef struct _ControlBind {
	uint16_t	controlId;
	uint16_t	padding;
	Control * 	controlPtr;
} ControlBind;

typedef struct _Page {
	Visual *		visualPtr;
	uint16_t		count;
	uint16_t		padding;
	ControlBind 	controlBind[1];
} Page;

typedef struct _PageBind {
	char * 	pageNamePtr;
	Page * 	pagePtr;
} PageBind;

typedef struct _Application {
	Visual* 	visualPtr;
	uint16_t 	count;
	uint16_t 	padding;
	PageBind 	pages[1];
} Application;

typedef struct _ApplicationBind {
	char*			appNamePtr;
	Application *	applicationPtr;
} ApplicationBind;

typedef struct _ConfigData {
	uint16_t count;
	uint16_t padding;
	ApplicationBind appRef[1];
} ConfigData;

#pragma pack(pop)

#endif //INC_AERGIA_AERGIA_TYPES_H

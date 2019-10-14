#pragma once

#include <windows.h>

typedef struct bushData {
	
// info state:
	BOOLEAN noiseGen; // noise generator or ASK
	BOOLEAN relayState;
	BOOLEAN tempOverHeat;
	BOOLEAN lockState;
	BOOLEAN doorState;
// temp sensors:
	INT8 averageTemp;
	INT8 firstTempSens;
	INT8 secondTempSens;
	INT8 thirdTempSens;
	INT8 fourthTempSens;

	enum OPCODE {
	// to bush
		STATE_GET = 0x01,
		TEMP_GET,
		CONNECT_CHECK = 0x04,
		LOCK_CHANGE,
		RELAY_CHANGE = 0x07,
    // from bush
        TEMP_SENS_AVERAGE = 0x80, 
		STATE_INFO,
		STATE_CHANGE = 0x83,
		CONNECT_FINE,
		BUTTON_PUSH = 0x86,
		TEMP_SENS_ONE = 0x91,
		TEMP_SENS_TWO,
		TEMP_SENS_THREE,
		TEMP_SENS_FOUR,
		ALERT_TEMP_SENS = 0xA1,
		ALERT_BISH_BRISH,
		ALERT_TEMP_OVERHEAT
	} lastOpcode;

	enum INFO_BYTE {
		NO_INFO,
		TEMP_AVERAGE = 0x80,
		TEMP_ALL = 0x90,
		TEMP_FIRST,
		TEMP_SECOND,
		TEMP_THIRD,
		TEMP_FOURTH,
		ON = 0x00,
		OFF = 0xFF
	};
} DATABUSH, *LPDATABUSH;

class BushData
{
public:
	BushData();
	~BushData();
};


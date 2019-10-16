#pragma once

#include <windows.h>

enum INFO_BYTE_BITS {
	DOOR,
	LOCK,
	OVERHEAT,
	RELAY,
	NOISE,
	RES1,
	RES2,
	RES3,
	BYTE_COUNT
};

typedef struct {
// info state:	
	BOOLEAN info[INFO_BYTE_BITS::BYTE_COUNT];
// temp sensors:
	INT8 averageTemp;
	INT8 firstTempSens;
	INT8 secondTempSens;
	INT8 thirdTempSens;
	INT8 fourthTempSens;

} DATABUSH, *LPDATABUSH;

enum BUSH_STATUS {
	NO_STATUS,
	DISCONNECTED,
	CONNECTED = 0x84, //similar to bush Opcode
	HEAT_SENS_ERR = 0xA1, 
	BUSH_BRISH_ERR,
	OVERHEATED
};

class BushData
{
private:
	DATABUSH bushState;
	BUSH_STATUS status;
	
	enum COMMAND_TO_BUSH
	{
		NO_COMMAND,
		CONNECT,
		DISCONNECT,
		OPEN_LOCK,
		CLOSE_LOCK,
		GET_TEMPRETURE
	} command;
	
	HANDLE hInfoChanged;
	HANDLE hInfoMutex;
	HANDLE hCommandEvent;
public:

	BushData() {
		SecureZeroMemory( &bushState, sizeof( DATABUSH ) );
		status = BUSH_STATUS::NO_STATUS;
		command = COMMAND_TO_BUSH::NO_COMMAND;
		hInfoChanged = CreateEvent( NULL, TRUE, FALSE, NULL );
		hCommandEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		hInfoMutex = CreateMutex( NULL, FALSE, NULL );
	}

	~BushData() {
		CloseHandle( hInfoChanged );
		CloseHandle( hInfoMutex );
		CloseHandle( hCommandEvent );
	}

	BOOL const IsDataChanged();
	HANDLE GetCommandEvent() {
		return hCommandEvent;
	}
	
	DWORD SetData( const DATABUSH& dataToSave );
	DWORD SetData( const BUSH_STATUS& statusToSave );
	DWORD SetData( const DATABUSH& dataToSave, const BUSH_STATUS & statusToSave );

	DWORD const GetData( DATABUSH& dataReturn, BUSH_STATUS& statusReturn );

};


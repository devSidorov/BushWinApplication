#pragma once

#include <windows.h>

typedef struct {
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

} DATABUSH, *LPDATABUSH;

class BushData
{
private:
	DATABUSH bushState;
	enum COMMAND_TO_BUSH
	{
		NO_COMMAND,
		CONNECT,
		DISCONNECT,
		OPEN_LOCK,
		CLOSE_LOCK,
		GET_TEMPRETURE
	} command;

	enum BUSH_STATUS {
		NO_INFO,
		CONNECTED,
		READY,
		DISCONNECTED
	} status;

	HANDLE hInfoChanged;
	HANDLE hInfoMutex;
	HANDLE hCommandEvent;
public:

	BushData() {
		SecureZeroMemory( &bushState, sizeof( DATABUSH ) );
		hInfoChanged = CreateEvent( NULL, TRUE, FALSE, NULL );
		hCommandEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		hInfoMutex = CreateMutex( NULL, FALSE, NULL );
	}

	~BushData() {
		CloseHandle( hInfoChanged );
		CloseHandle( hInfoMutex );
		CloseHandle( hCommandEvent );
	}

	BOOL IsDataChanged();
	
	DWORD SetData( DATABUSH& dataToSave );
	DWORD GetData( DATABUSH& dataReturn );

	DWORD SetStatus( BUSH_STATUS& );
	BUSH_STATUS GetStatus();

	HANDLE GetCommandEvent() { 
		return hCommandEvent;
	}
};


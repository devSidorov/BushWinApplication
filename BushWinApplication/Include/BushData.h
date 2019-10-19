#pragma once

#include <windows.h>

#include "SerialPortBush.h"

class BushData
{
private:
	DATABUSH bushState;
	BUSH_STATUS status;	
	BUSH_SCRIPT command;
	
	HANDLE hInfoChanged;
	HANDLE hInfoMutex;
	HANDLE hCommandEvent;
public:

	BushData() {
		SecureZeroMemory( &bushState, sizeof( DATABUSH ) );
		status = BUSH_STATUS::NO_STATUS;
		command = BUSH_SCRIPT::NO_SCRIPT;
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


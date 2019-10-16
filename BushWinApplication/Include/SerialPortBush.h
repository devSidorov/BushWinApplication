#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include "BushData.h"

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
};

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

enum {
	FIRST_BYTE,
	OPCODE_BYTE,
	INFO_BYTE,
	CACHE_BYTE
};
const BYTE firstByte = 0xAA;
const BYTE BYTES_IO = 4;
const BYTE INFO_BYTES = 2;

class SerialPortBush
{
private:
	
	BUSH_STATUS bushStatus;
	DATABUSH bushState;
	TCHAR caPortName[MAX_PATH];
	HANDLE hCom;

public:
	DWORD SetPortName( const TCHAR* pcPortName ) {
		if ( pcPortName )
			return _tcscpy_s<MAX_PATH>( caPortName, pcPortName );
		
		return EINVAL;
	}

	SerialPortBush() {
		bushStatus = BUSH_STATUS::NO_STATUS;
		SecureZeroMemory( &caPortName, sizeof( TCHAR ) * MAX_PATH );
		SecureZeroMemory( &bushState, sizeof( DATABUSH ) );
		hCom = INVALID_HANDLE_VALUE;
	}
	SerialPortBush( const TCHAR* pcPortName ) : SerialPortBush() {
		SetPortName( pcPortName );
	}
	~SerialPortBush();

private:
	DWORD ConnectPort();
	DWORD ConfigPort();
	BYTE ReadPort( BYTE& opcodeByte, BYTE& infoByte, BOOL firstRead = TRUE );
	DWORD WritePort( const BYTE opcodeByte, const BYTE infoByte );

	DWORD ParseInput( BYTE opcodeByte, BYTE infoByte );
	DWORD ParseStateByte( BYTE infoByte );
	DWORD ParseChangeByte( BYTE infoByte );

	BYTE dallas_crc8( const BYTE * dataCheck = NULL, UINT sizeData = 0 );

public:
	DWORD Open();
	DWORD Read();
	DWORD Read( BYTE checkOpcode );
	
	BUSH_STATUS GetStatus() {
		return bushStatus;
	}
	const DATABUSH& GetState() { 
		return bushState;
	}
	
	DWORD Write( const BYTE opcodeByte, const BYTE infoByte );
		
	HANDLE GetHandle() { 
		return hCom; 
	}
	
};


#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>

enum BUSH_STATUS {
	NO_STATUS,
	DISCONNECTED,
	CONNECTED = 0x84, //similar to bush Opcode
	HEAT_SENS_ERR = 0xA1,
	BUSH_BRISH_ERR,
	OVERHEATED
};

enum OPCODE {
	NOT_VALUE,
	// to bush
	STATE_GET,
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
	OFF = 0x00,
	ON = 0xFF
};

enum INFO_BYTE_BITS {
	DOOR,
	LOCK,
	OVERHEAT,
	RELAY,
	NOISE,
	RES1,
	RES2,
	RES3,
	BIT_COUNT
};

enum BUSH_SCRIPT
{
	NO_SCRIPT,
	INIT,
	LOCK_LOCK, // this and LOCK_UNLOCK must be on next to another
	LOCK_UNLOCK,
	RELAY_ON, // this and RELAY_OFF must be on next to another
	RELAY_OFF,
	GET_TEMPRETURE,
	DISCONNECT //TODO delete if dont need
};

typedef struct {
	// info state:	
	BOOLEAN info[INFO_BYTE_BITS::BIT_COUNT];
	// temp sensors:
	INT8 averageTemp;
	INT8 firstTempSens;
	INT8 secondTempSens;
	INT8 thirdTempSens;
	INT8 fourthTempSens;

} DATABUSH, *LPDATABUSH;

enum INPUT_PACKET{
	FIRST_BYTE,
	OPCODE_BYTE,
	INFO_BYTE,
	CACHE_BYTE,
	COUNT_BYTE
};

struct DATA_FROM_BUSH {
	BYTE opcodeByte;
	BYTE infoByte;
};

const BYTE FIRST_BYTE_VALUE = 0xAA;
const BYTE INFO_BYTES = 2;
const INT8 maxStack = 10;

class SerialPortBush
{
private:
	HANDLE m_hReadThreadHandle;
	HANDLE m_hMutexReadData;
	HANDLE m_hDataFromBush;
	HANDLE m_hReadThreadStop; //TODO add flag pushing
	DATA_FROM_BUSH m_aDataReadITC[maxStack]; // TODO rewrite class to protect this data
	INT8 m_dCurrent;
	INT8 m_dLastRead;

protected:	
	BUSH_STATUS bushStatus;
	DATABUSH bushState;
	TCHAR caPortName[MAX_PATH];
	HANDLE hCom;

public:
	SerialPortBush() {
		m_hReadThreadHandle = nullptr;
		m_hMutexReadData = CreateMutex( nullptr, FALSE, nullptr );
		m_hDataFromBush = CreateEvent( nullptr, TRUE, FALSE, nullptr );
		m_hReadThreadStop = CreateEvent( nullptr, TRUE, FALSE, nullptr );
		SecureZeroMemory( m_aDataReadITC, sizeof( DATA_FROM_BUSH ) * maxStack );
		m_dCurrent = m_dLastRead = -1;
		
		bushStatus = BUSH_STATUS::NO_STATUS;
		SecureZeroMemory( &caPortName, sizeof( TCHAR ) * MAX_PATH );
		SecureZeroMemory( &bushState, sizeof( DATABUSH ) );
		hCom = INVALID_HANDLE_VALUE;
	}
	SerialPortBush( const TCHAR* pcPortName ) : SerialPortBush() {
		SetPortName( pcPortName );
	}
	~SerialPortBush() { 
		CloseHandle( hCom );
	}

private:
	DWORD SetPortName( const TCHAR* pcPortName ) {
		if ( pcPortName )
			return _tcscpy_s<MAX_PATH>( caPortName, pcPortName );

		return EINVAL;
	}

	DWORD ConnectPort();
	DWORD ConfigPort();
	DWORD StartReadThread();
	DWORD ReadPort( BYTE& opcodeByte, BYTE& infoByte );
	DWORD WritePort( const BYTE opcodeByte, const BYTE infoByte );
	
	DWORD PutDataITC( const DATA_FROM_BUSH & dataToPut );
	DWORD const GetDataITC( DATA_FROM_BUSH & dataGet );

	DWORD ParseInput( BYTE opcodeByte, BYTE infoByte );
	DWORD ParseStateByte( BYTE infoByte );
	DWORD ParseChangeByte( BYTE infoByte );


	BYTE dallas_crc8( const BYTE * dataCheck = NULL, UINT sizeData = 0 );

public:
	DWORD Open();
	DWORD Write( const BYTE opcodeByte, const BYTE infoByte );
	
	DWORD ReadToITData();
	DWORD ReadFromITData();

	BUSH_STATUS GetStatus() {
		return bushStatus;
	}
	const DATABUSH& GetState() { 
		return bushState;
	}
	
	HANDLE const GetEventDataFromBush() {
		return m_hDataFromBush;
	}
	BOOL const fnIsReadThreadNeed()
	{
		if ( WaitForSingleObject( m_hReadThreadStop, NULL ) == WAIT_OBJECT_0 )
			return FALSE;
		return TRUE;
	}	
};

DWORD WINAPI fnFromBushThread( LPVOID lpParam );


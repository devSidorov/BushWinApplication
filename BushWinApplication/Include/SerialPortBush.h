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
const INT8 MAX_STACK = 10;
const INT8 TEMPERATURE_STAB = 41;
const INT8 MIN_OVERHEAT_TEMP = 32;
const WORD M_WAIT_TIME_DEFAULT = 1000;

class SerialPortBush
{
private:
	HANDLE m_hMutexReadData;
	HANDLE m_hDataFromBush;
	HANDLE m_hReadThreadStop; //TODO add flag pushing
	DATA_FROM_BUSH m_aDataReadITC[MAX_STACK]; // TODO rewrite class to protect this data
	INT8 m_dCurrent;
	INT8 m_dLastRead;

protected:	
	BUSH_STATUS m_bushStatus;
	DATABUSH m_bushState;
	TCHAR m_acPortName[MAX_PATH];
	HANDLE m_hComPort;
	HANDLE m_hReadThreadHandle;

public:
	SerialPortBush() {
		m_hReadThreadHandle = nullptr;
		m_hMutexReadData = CreateMutex( nullptr, FALSE, nullptr );
		m_hDataFromBush = CreateEvent( nullptr, TRUE, FALSE, nullptr );
		m_hReadThreadStop = CreateEvent( nullptr, TRUE, FALSE, nullptr );
		SecureZeroMemory( m_aDataReadITC, sizeof( DATA_FROM_BUSH ) * MAX_STACK );
		m_dCurrent = m_dLastRead = -1;
		
		m_bushStatus = BUSH_STATUS::NO_STATUS;
		SecureZeroMemory( &m_acPortName, sizeof( TCHAR ) * MAX_PATH );
		SecureZeroMemory( &m_bushState, sizeof( DATABUSH ) );
		m_hComPort = INVALID_HANDLE_VALUE;
	}
	SerialPortBush( const TCHAR* pcPortName ) : SerialPortBush() {
		fnSetPortName( pcPortName );
	}
	~SerialPortBush() { 
		fnReleaseHandles();
	}

private:
	VOID fnReleaseHandles() {
		CloseHandle( m_hReadThreadHandle );
		CloseHandle( m_hMutexReadData );
		CloseHandle( m_hDataFromBush );
		CloseHandle( m_hReadThreadStop );
		CloseHandle( m_hComPort );
	}
	
	DWORD fnSetPortName( const TCHAR* pcPortName ) {
		if ( *pcPortName )
			_tcscpy_s<MAX_PATH>( m_acPortName, pcPortName );

		return ERROR_SUCCESS;
	}

	DWORD fnConnectPort();
	DWORD fnConfigPort();
	DWORD fnStartReadThread();
	DWORD fnStopReadThread();
	DWORD fnReadPort( BYTE& opcodeByte, BYTE& infoByte );
	DWORD fnWritePort( const BYTE opcodeByte, const BYTE infoByte );
	
	DWORD fnPutDataITC( const DATA_FROM_BUSH & dataToPut );
	DWORD const fnGetDataITC( DATA_FROM_BUSH & dataGet );

	INT8 fnAverageTempCalc( const BYTE infoByte );
	DWORD fnParseInput( BYTE opcodeByte, BYTE infoByte );
	DWORD fnParseStateByte( BYTE infoByte );
	DWORD fnParseChangeByte( BYTE infoByte );


	BYTE fnDallasMaximCRC8( const BYTE * dataCheck = NULL, UINT sizeData = 0 );

public:
	DWORD fnOpen();
	DWORD fnClose();
	DWORD fnWrite( const BYTE opcodeByte, const BYTE infoByte );
	
	DWORD fnReadToITData();
	DWORD fnReadFromITData();

	HANDLE const fnGetEventDataFromBush() {
		return m_hDataFromBush;
	}
	BOOL const fnIsReadThreadNeed();
	
};

DWORD WINAPI fnFromBushThread( LPVOID lpParam );


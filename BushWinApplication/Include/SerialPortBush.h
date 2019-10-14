#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>



const BYTE BYTES_IO = 4;
const BYTE INFO_BYTES = 2;
enum {
	FIRST_BYTE,
	OPCODE_BYTE,
	INFO_BYTE,
	CACHE_BYTE
};
const BYTE firstByte = 0xAA;

class SerialPortBush
{
private:

	TCHAR caPortName[MAX_PATH];
	HANDLE hCom;

public:
	DWORD SetPortName( const TCHAR* pcPortName ) {
		if ( pcPortName )
			return _tcscpy_s<MAX_PATH>( caPortName, pcPortName );
		
		return EINVAL;
	}

	SerialPortBush() {
		SecureZeroMemory( &caPortName, sizeof( TCHAR ) * MAX_PATH );
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

	BYTE dallas_crc8( const BYTE * dataCheck = NULL, UINT sizeData = 0 );

public:
	DWORD Open();
	DWORD Read();
	DWORD Write( const BYTE opcodeByte, const BYTE infoByte );
	
	HANDLE GetHandle() { 
		return hCom; 
	}
	
};


#include "SerialPortBush.h"

DWORD SerialPortBush::ConnectPort()
{
	
	if ( *caPortName )
	{
		hCom = CreateFile( caPortName,
						   GENERIC_READ | GENERIC_WRITE,
						   NULL,      //  must be opened with exclusive-access
						   NULL,   //  default security attributes
						   OPEN_EXISTING, // serial port default
						   NULL,
						   NULL ); // OPEN_EXISTING default 		
	}
	
	return ( hCom != INVALID_HANDLE_VALUE )? ERROR_SUCCESS : ERROR_PORT_UNREACHABLE;
}

DWORD SerialPortBush::ConfigPort()
{
	_DCB portConfig;
	
	//set setting from "BUSH" application note
	SecureZeroMemory( &portConfig, sizeof( _DCB ) );
	portConfig.DCBlength = sizeof( _DCB );
	portConfig.BaudRate = CBR_115200;
	portConfig.fBinary = TRUE; //Windoes default
	portConfig.ByteSize = 0x08; //Standart packet

	DWORD fSuccess = SetCommState( hCom, &portConfig );
	System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Setting config for port! {0:X}", GetLastError() ) );
	if ( !fSuccess )
		return ERROR_PORT_NOT_SET;
	
	//set event - first char from port
	fSuccess = SetCommMask( hCom, EV_RXCHAR );
	System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Setting mask config for port! {0:X}", GetLastError() ) );
	if ( !fSuccess )
		return ERROR_PORT_NOT_SET;

	//fSuccess = PurgeComm( hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR ); // clear buffer
	//System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Clearing buffer port! {0:X}", GetLastError() ) );

	return ERROR_SUCCESS;
}

//Read input buffer, dont log firstbyte error because its signal of clean buffer
DWORD SerialPortBush::ReadPort( BYTE& opcodeByte, BYTE& infoByte ,BOOL firstRead )
{
	BYTE bufferRead[COUNT_BYTE] = { 0,0,0,0 };
	DWORD bytesIOoperated;
	
	for ( INT8 inc = 0; inc <= CACHE_BYTE; inc++ )
	{
		DWORD fSuccess = ReadFile( hCom,
								   bufferRead + inc,
								   1,
								   &bytesIOoperated,
								   NULL );
		//check input bytes
		if ( fSuccess )
		{
			switch ( inc )
			{
			case FIRST_BYTE:
				if ( bufferRead[inc] != FIRST_BYTE_VALUE )
					return ERROR_DATA_NOT_ACCEPTED;
				break;
			case OPCODE_BYTE:
				if ( !bufferRead[inc] )
					return ERROR_DATA_NOT_ACCEPTED;
				break;
			case INFO_BYTE:
				break;
			case CACHE_BYTE:
				if ( bufferRead[CACHE_BYTE] != dallas_crc8( bufferRead + 1, INFO_BYTES ) )
					return ERROR_DATA_NOT_ACCEPTED;
				else
				{
					opcodeByte = bufferRead[OPCODE_BYTE];
					infoByte = bufferRead[INFO_BYTE];
				}
				break;
			default:
				System::Diagnostics::Debug::Assert( TRUE, "ERROR! Read from port byte ckeck mustnot be here" );
				return ERROR_UNIDENTIFIED_ERROR;
			}
		}
		else
		{
			System::Diagnostics::Debug::Assert( TRUE, "ERROR! Read from port failed" ); //TODO normal error handling
			return ERROR_PORT_UNREACHABLE;;
		}
	}		
	return ERROR_SUCCESS;
}

DWORD SerialPortBush::WritePort( BYTE opcodeByte, BYTE infoByte )
{
	BYTE bufferWrite[COUNT_BYTE] = { FIRST_BYTE_VALUE, opcodeByte, infoByte, 0 };
	DWORD bytesIOoperated;
	
	bufferWrite[3] = dallas_crc8( bufferWrite + 1, INFO_BYTES );

	DWORD fSuccess = WriteFile( hCom,
								bufferWrite,
								COUNT_BYTE,
								&bytesIOoperated,
								NULL );
	
	System::Diagnostics::Debug::Assert( fSuccess, "ERROR! Write to port failed" ); //TODO normal error handling
	
	return 0;
}

DWORD SerialPortBush::Open()
{
	DWORD fSuccess = ConnectPort();
	if ( !fSuccess )
		fSuccess = ConfigPort();	

	return fSuccess;
}

//reads input and parses result into data struct and status value
DWORD SerialPortBush::Read( DWORD& opcodeReaden )
{
	BYTE opcodeByte = 0;
	BYTE infoByte = 0;
	DWORD fSuccess = 0;
	DWORD evtMask = 0;

	fSuccess = WaitCommEvent( hCom, &evtMask, NULL );
	if ( !fSuccess )
	{
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wait for comm event fail! {0:X}", fSuccess = GetLastError() ) );
		return fSuccess;
	}

	fSuccess = ReadPort( opcodeByte, infoByte );
	if ( !fSuccess )
	{
		ParseInput( opcodeByte, infoByte );
		opcodeReaden = opcodeByte;
	}		
	
	return fSuccess;
}

DWORD SerialPortBush::ParseStateByte( BYTE infoByte )
{
	for ( int bitPosition = INFO_BYTE_BITS::DOOR; bitPosition <= INFO_BYTE_BITS::NOISE; bitPosition++ )
		bushState.info[bitPosition] = ( infoByte >> bitPosition )&0x01;
	
	return ERROR_SUCCESS;
}

DWORD SerialPortBush::ParseChangeByte( BYTE infoByte )
{
	for ( int bitPosition = INFO_BYTE_BITS::DOOR; bitPosition <= INFO_BYTE_BITS::NOISE; bitPosition++ )
		if ( ( infoByte >> bitPosition ) & 0x01 )
		{
			bushState.info[bitPosition] = !bushState.info[bitPosition];
			return ERROR_SUCCESS;
		}
		
	System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wrong info opcode from bush! {0:X}", infoByte ) );
	return ERROR_INVALID_PARAMETER;
}

DWORD SerialPortBush::ParseInput( BYTE opcodeByte, BYTE infoByte )
{
	switch ( opcodeByte )
	{
	case OPCODE::TEMP_SENS_AVERAGE:
		bushState.averageTemp = infoByte; //TODO change temp handling
		break;
	case OPCODE::STATE_INFO:
		ParseStateByte( infoByte );
		break;
	case OPCODE::STATE_CHANGE:
		ParseChangeByte( infoByte );
		break;
	case OPCODE::CONNECT_FINE:
		bushStatus = BUSH_STATUS::CONNECTED;
		System::Diagnostics::Debug::Assert( !infoByte, System::String::Format( "ERROR! Wrong info opcode from bush! {0:X}", infoByte ) );
		break;
	case OPCODE::BUTTON_PUSH:
		//TODO add implementation
		break;
	case OPCODE::TEMP_SENS_ONE:
		bushState.firstTempSens = infoByte;
		break;
	case OPCODE::TEMP_SENS_TWO:
		bushState.secondTempSens = infoByte;
		break;
	case OPCODE::TEMP_SENS_THREE:
		bushState.thirdTempSens = infoByte;
		break;		
	case OPCODE::TEMP_SENS_FOUR:
		bushState.fourthTempSens = infoByte;
		break;
	case OPCODE::ALERT_TEMP_SENS:
		bushStatus = BUSH_STATUS::HEAT_SENS_ERR;
		//TODO add implementation for infobyte		
		System::Diagnostics::Debug::Assert( !infoByte, System::String::Format( "ERROR! Wrong info opcode from bush! {0:X}", infoByte ) );
		break;
	case OPCODE::ALERT_BISH_BRISH:
		bushStatus = BUSH_STATUS::BUSH_BRISH_ERR;
		System::Diagnostics::Debug::Assert( !infoByte, System::String::Format( "ERROR! Wrong info opcode from bush! {0:X}", infoByte ) );
		break;
	case OPCODE::ALERT_TEMP_OVERHEAT:		
		bushStatus = BUSH_STATUS::OVERHEATED;
		System::Diagnostics::Debug::Assert( !infoByte, System::String::Format( "ERROR! Wrong info opcode from bush! {0:X}", infoByte ) );
		break;
	case OPCODE::NOT_VALUE:
		break;
	default:
		//TODO add throw to place where will be state change
		//TODO add log
		//TODO error if open close door fast
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wrong opcode from bush! {0:X}", opcodeByte ) );
		break;
	}
	
	return 0;
}

DWORD SerialPortBush::Write( const BYTE opcodeByte, const BYTE infoByte )
{
	WritePort( opcodeByte, infoByte );
	return 0;
}





BYTE SerialPortBush::dallas_crc8( const BYTE * dataCheck, UINT sizeData )
{
	BYTE crcArray[256] = {
		0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
		0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
		0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
		0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
		0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
		0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
		0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
		0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
		0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
		0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
		0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
		0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
		0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
		0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
		0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
		0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
		0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
		0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
		0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
		0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
		0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
		0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
		0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
		0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
		0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
		0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
		0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
		0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
		0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
		0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
		0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
		0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
	};
	BYTE crcRes = 0;

	for ( UINT incLoop = 0; incLoop < sizeData; incLoop++ )
	{
		crcRes = crcArray[dataCheck[incLoop] ^ crcRes];
	}
	return crcRes;
}


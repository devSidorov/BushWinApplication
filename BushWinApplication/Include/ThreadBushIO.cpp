#include "ThreadBushIO.h"

// Thread waiting for first char in serial port buffer
// It is one of event that trigger actions in IO between PC and Bush 
DWORD WINAPI FromBushThread( LPVOID lpParam )
{
	HANDLE hCom = ( HANDLE )lpParam;
	DWORD evtMask = NULL;

	DWORD fSuccess = WaitCommEvent( hCom, &evtMask, NULL );
	System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Waiting event from port! {0:X}", GetLastError() )  );
	
	return evtMask;
}


DWORD WINAPI MainIOBushThread( LPVOID lpParam )
{
	const DWORD evCount = 2;
	enum EVENT_ARR {
		COMMAND,
		BUSH_INPUT
	};
	
	LPINTHREADDATA pDataParams = ( LPINTHREADDATA )lpParam;

	BushData* pDataITC = ( ( LPINTHREADDATA )lpParam )->pBushData;
	SerialPortBush bushPort( ( ( LPINTHREADDATA )lpParam )->pPortName );
	DWORD fSuccess = bushPort.Open();
	System::Diagnostics::Debug::Assert( !fSuccess, "ERROR! While openning port!" );	
	
	HANDLE haEvHandler[evCount] = { 0, 0 };
	haEvHandler[COMMAND] = pDataITC->GetCommandEvent();
	HANDLE hComPort = bushPort.GetHandle();

	DWORD dWaitTime = INFINITE;

	while ( TRUE )
	{
		//Thread waiting for first character in input buffer
		haEvHandler[EVENT_ARR::BUSH_INPUT] = CreateThread( NULL, 0, FromBushThread, hComPort, 0, NULL );
		System::Diagnostics::Debug::Assert( haEvHandler[EVENT_ARR::BUSH_INPUT], "ERROR! Starting wait from bush thread!" );

		fSuccess = WaitForMultipleObjects( evCount,
										   haEvHandler,
										   FALSE,
										   dWaitTime );
		switch ( fSuccess )
		{
		case WAIT_OBJECT_0: //CommandEvent
			TerminateThread( haEvHandler, NULL );
			
			break;
		case ( WAIT_OBJECT_0 + EVENT_ARR::BUSH_INPUT ): //Something from Bush
			InputBushHandle( bushPort, pDataITC );
			break;
		case WAIT_ABANDONED_0:
		case ( WAIT_ABANDONED_0 + EVENT_ARR::BUSH_INPUT ):
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		default:
			System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wait for events returned {0:X} error {0:X}", fSuccess, GetLastError() ) );
		}
	}

	//TODO add status and its processing then thread is ended for gui
	return ERROR_SUCCESS;
}

DWORD InputBushHandle( SerialPortBush& bushPort, BushData* pDataITC )
{ 
	bushPort.Read();
	pDataITC->SetData( bushPort.GetState(), bushPort.GetStatus() );

	return ERROR_SUCCESS;
}


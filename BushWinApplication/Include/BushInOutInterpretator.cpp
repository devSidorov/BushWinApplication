#include "BushInOutInterpretator.h"



DWORD BushInOutInterpretator::Start()
{
	DWORD fSuccess = Open();
	System::Diagnostics::Debug::Assert( !fSuccess, "ERROR! While openning port!" );
	
	Write( OPCODE::CONNECT_CHECK, INFO_BYTE::NO_INFO );
	pDataITC->SetData( BUSH_STATUS::DISCONNECTED );
	BYTE waitForOpcode = OPCODE::CONNECT_FINE;
	DWORD dWaitTime = 1000;

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::Finish()
{
	//TODO add port close and memory clean if needed
	//TODO add command send that thread is ended
	return 0;
}

BOOL BushInOutInterpretator::WaitForNextIO()
{
	BOOL IsCommandNotDissconnect = TRUE;

	haEvHandler[EVENT_ARR::BUSH_INPUT] = CreateThread( NULL, 0, FromBushThread, &hCom, 0, NULL );
	System::Diagnostics::Debug::Assert( haEvHandler[EVENT_ARR::BUSH_INPUT], "ERROR! Starting wait from bush thread!" );
	
	DWORD fSuccess = WaitForMultipleObjects( EV_COUNT,
											 haEvHandler,
											 FALSE,
											 dWaitTime );
	switch ( fSuccess )
	{
	case WAIT_OBJECT_0: //CommandEvent
		TerminateThread( haEvHandler[EVENT_ARR::BUSH_INPUT], NULL );
		break;
	case ( WAIT_OBJECT_0 + EVENT_ARR::BUSH_INPUT ): //Something from Bush
		InputBushHandle();
		break;
	case WAIT_ABANDONED_0:
	case ( WAIT_ABANDONED_0 + EVENT_ARR::BUSH_INPUT ):
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	default:
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wait for events returned {0:X} error {0:X}", fSuccess, GetLastError() ) );
	}

	return IsCommandNotDissconnect;
}

DWORD BushInOutInterpretator::InputBushHandle()
{
	Read();
	pDataITC->SetData( bushState, bushStatus );

	return ERROR_SUCCESS;
}

DWORD __stdcall FromBushThread( LPVOID lpParam )
{
	{
		const HANDLE hComPort = *( ( HANDLE* )lpParam );
		DWORD evtMask = NULL;

		DWORD fSuccess = WaitCommEvent( hComPort, &evtMask, NULL );
		System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Waiting event from port! {0:X}", GetLastError() ) );

		return evtMask;
	}
}

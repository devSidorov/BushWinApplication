#include "BushInOutInterpretator.h"



DWORD BushInOutInterpretator::Start()
{
	DWORD fSuccess = Open();
	System::Diagnostics::Debug::Assert( !fSuccess, "ERROR! While openning port!" );
	
	ConnectCheck();
	return ERROR_SUCCESS;
}


DWORD BushInOutInterpretator::Finish()
{
	//TODO add port close and memory clean if needed
	//TODO add command send that thread is ended
	return 0;
}

DWORD BushInOutInterpretator::DefaultWait()
{
	waitForOpcode = OPCODE::NOT_VALUE;
	dWaitTime = INFINITE; 

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::ConnectCheck()
{
	Write( OPCODE::CONNECT_CHECK, INFO_BYTE::NO_INFO );
	pDataITC->SetData( BUSH_STATUS::DISCONNECTED );
	script = BUSH_SCRIPT::INIT;
	waitForOpcode = OPCODE::CONNECT_FINE;
	dWaitTime = 1000;

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::AskState()
{
	Write( OPCODE::STATE_GET, INFO_BYTE::NO_INFO );
	waitForOpcode = OPCODE::STATE_INFO;
	dWaitTime = 1000;

	return ERROR_SUCCESS;
}


BOOL BushInOutInterpretator::WaitForNextIO()
{
	BOOL IsCommandNotDissconnect = TRUE;

	haEvHandler[EVENT_ARR::BUSH_INPUT] = CreateThread( NULL, 0, FromBushThread, &hCom, 0, NULL );
	System::Diagnostics::Debug::Assert( haEvHandler[EVENT_ARR::BUSH_INPUT], "ERROR! Starting wait from bush thread!" );
	
	//TODO add state check timer
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
	case WAIT_TIMEOUT:
		TerminateThread( haEvHandler[EVENT_ARR::BUSH_INPUT], NULL );
		break;
	case WAIT_ABANDONED_0:
	case ( WAIT_ABANDONED_0 + EVENT_ARR::BUSH_INPUT ):	
	case WAIT_FAILED:
	default:
		TerminateThread( haEvHandler[EVENT_ARR::BUSH_INPUT], NULL );
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wait for events returned {0:X} error {0:X}", fSuccess, GetLastError() ) );
	}

	return IsCommandNotDissconnect;
}

DWORD BushInOutInterpretator::InputBushHandle()
{
	DWORD fSuccess = 0;
	
	fSuccess = Read( waitForOpcode );
	if ( fSuccess )
	{
		switch ( script )
		{
		case BUSH_SCRIPT::NO_SCRIPT:
			System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! If its no script read must not be here" ) );
			break;
		case BUSH_SCRIPT::INIT:
			if ( waitForOpcode == OPCODE::CONNECT_FINE )
				AskState();
			else if ( waitForOpcode == OPCODE::STATE_INFO )
			{
				pDataITC->SetData( bushState, bushStatus );
				DefaultWait();
			}			
			else 
				System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Must not be here script -{0:X}", ( DWORD )script ) );
			break;
		case BUSH_SCRIPT::LOCK_LOCK:
		case BUSH_SCRIPT::LOCK_UNLOCK:
			break;
		case BUSH_SCRIPT::RELAY_LOCK:
		case BUSH_SCRIPT::RELAY_UNLOCK:
			break;
		case BUSH_SCRIPT::GET_TEMPRETURE:
			break;
		}
	}
	else
	{
		// if no wait its free state, and bush can give new status, if waiting for opcode only data must change
		if ( waitForOpcode )
			pDataITC->SetData( bushState );
		else
			pDataITC->SetData( bushState, bushStatus );
	}
	
	return ERROR_SUCCESS;
}

DWORD WINAPI FromBushThread( LPVOID lpParam )
{
	{
		const HANDLE hComPort = *( ( HANDLE* )lpParam );
		DWORD evtMask = NULL;

		DWORD fSuccess = WaitCommEvent( hComPort, &evtMask, NULL );
		System::Diagnostics::Debug::Assert( fSuccess, System::String::Format( "ERROR! Waiting event from port! {0:X}", GetLastError() ) );

		return evtMask;
	}
}

DWORD WINAPI MainIOBushThread( LPVOID lpParam )
{
	BushInOutInterpretator BushConnect( ( ( LPINTHREADDATA )lpParam )->pBushData, ( ( LPINTHREADDATA )lpParam )->pPortName );
	BushConnect.Start();

	while ( BushConnect.WaitForNextIO() );

	BushConnect.Finish();
	return ERROR_SUCCESS;
}
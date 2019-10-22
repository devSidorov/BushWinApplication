#include "BushInOutInterpretator.h"



DWORD BushInOutInterpretator::fnStart()
{
	DWORD fSuccess = Open();
	System::Diagnostics::Debug::Assert( !fSuccess, "ERROR! While openning port!" );
	
	fnConnectCheck();
	return ERROR_SUCCESS;
}


DWORD BushInOutInterpretator::fnFinish()
{
	//TODO add port close and memory clean if needed
	//TODO add command send that thread is ended
	return 0;
}

DWORD BushInOutInterpretator::fnDefaultWait()
{
	m_script = BUSH_SCRIPT::NO_SCRIPT;
	m_waitForOpcode = OPCODE::NOT_VALUE;
	m_dwWaitTime = M_WAIT_TIME_UPDATE;
	m_bRepeatErr = FALSE;

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::fnConnectCheck()
{
	Write( OPCODE::CONNECT_CHECK, INFO_BYTE::NO_INFO );
	m_pDataITC->SetData( BUSH_STATUS::DISCONNECTED );
	m_script = BUSH_SCRIPT::INIT;
	m_waitForOpcode = OPCODE::CONNECT_FINE;
	m_dwWaitTime = M_WAIT_TIME_DEFAULT;
	m_bRepeatErr = FALSE;

	return ERROR_SUCCESS;
}

// function must be without script setting because some scripts use it to greed BUSH
DWORD BushInOutInterpretator::fnAskStateInit()
{
	Write( OPCODE::STATE_GET, INFO_BYTE::NO_INFO );
	m_waitForOpcode = OPCODE::STATE_INFO;
	m_dwWaitTime = M_WAIT_TIME_DEFAULT;
	m_bRepeatErr = FALSE;

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::fnAskState()
{
	m_script = BUSH_SCRIPT::INIT;
	fnAskStateInit();

	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::fnGetHeatSens()
{
	m_script = BUSH_SCRIPT::GET_TEMPRETURE;
	Write( OPCODE::TEMP_GET, INFO_BYTE::TEMP_AVERAGE );
	m_waitForOpcode = OPCODE::TEMP_SENS_AVERAGE;
	m_dwWaitTime = M_WAIT_TIME_DEFAULT;
	m_bRepeatErr = FALSE;

	return ERROR_SUCCESS;
}

// Script for change state lock and relay from bush documentationn
// parameters bOnMech and bIsLockScript play role only in first step, after function works with class member m_script
DWORD BushInOutInterpretator::fnLockRelayScript( SCRIPT_STEP wStep = SCRIPT_STEP::FIRST_STEP, BOOL bOnMech = TRUE, BOOL bIsLockScript = TRUE )
{
	if ( wStep == SCRIPT_STEP::FIRST_STEP )
	{
		if ( bIsLockScript )
			m_script = ( bOnMech ) ? BUSH_SCRIPT::LOCK_LOCK : BUSH_SCRIPT::LOCK_UNLOCK;
		else
			m_script = ( bOnMech ) ? BUSH_SCRIPT::RELAY_ON : BUSH_SCRIPT::RELAY_OFF;
		fnAskStateInit();
	}
	else if ( wStep == SCRIPT_STEP::SECOND_STEP )
	{
		Write( ( m_script == BUSH_SCRIPT::LOCK_LOCK || m_script == BUSH_SCRIPT::LOCK_UNLOCK ) ? OPCODE::LOCK_CHANGE : OPCODE::RELAY_CHANGE,
			   ( m_script == BUSH_SCRIPT::LOCK_LOCK || m_script == BUSH_SCRIPT::RELAY_ON ) ? INFO_BYTE::ON : INFO_BYTE::OFF );
		m_waitForOpcode = OPCODE::STATE_CHANGE;
		m_bRepeatErr = FALSE;
	}
	else if ( wStep == SCRIPT_STEP::THIRD_STEP )
	{
		if ( bushState.info[ ( m_script == BUSH_SCRIPT::LOCK_LOCK || m_script == BUSH_SCRIPT::LOCK_UNLOCK ) ? INFO_BYTE_BITS::LOCK : INFO_BYTE_BITS::RELAY ] 
			 == ( m_script == BUSH_SCRIPT::LOCK_LOCK || m_script == BUSH_SCRIPT::RELAY_ON ) ? 1 : 0 ) //check if state is changed as script asked
		{
			m_pDataITC->SetData( bushState, bushStatus );
			fnDefaultWait();
		}
		else; //waiting for another opcode, do nothing and wait for opcode or timer to send repeat of command
	}
	else
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "Unpredicted behavior! Script lock-relay, unknown step -{0:X}", ( DWORD )wStep ) );

	return ERROR_SUCCESS;
}


BOOL BushInOutInterpretator::fnWaitForNextIO()
{
	BOOL IsCommandNotDissconnect = TRUE;

	DWORD fSuccess = WaitForMultipleObjects( EV_COUNT,
											 m_haEvHandler,
											 FALSE,
											 m_dwWaitTime );
	switch ( fSuccess )
	{
	case WAIT_OBJECT_0: //CommandEvent
		break;
	case ( WAIT_OBJECT_0 + EVENT_ARR::BUSH_INPUT ): //Something from Bush
		fnInputBushHandle();
		break;
	case WAIT_TIMEOUT:
		fnTimerWaitHandle();
		break;
	case WAIT_ABANDONED_0:
	case ( WAIT_ABANDONED_0 + EVENT_ARR::BUSH_INPUT ):	
	case WAIT_FAILED:
	default:
		//TODO add processing of unpredicted events with threads
		System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Wait for events returned {0:X} error {0:X}", fSuccess, GetLastError() ) );
	}

	return IsCommandNotDissconnect;
}

DWORD BushInOutInterpretator::fnInputBushHandle()
{
	DWORD returnedOpcode = ReadFromITData();

	if ( returnedOpcode == m_waitForOpcode )
	{
		// Processing right returned opcode 
		switch ( m_script )
		{
		case BUSH_SCRIPT::NO_SCRIPT:
			System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "Unpredicted behavior! Input processing, no script, returned opcode -{0:X}", returnedOpcode ) );
			break;
		case BUSH_SCRIPT::INIT:
			if ( m_waitForOpcode == OPCODE::CONNECT_FINE )
				fnAskStateInit();
			else if ( m_waitForOpcode == OPCODE::STATE_INFO )
				fnGetHeatSens(); // get temp sensor before sending info to Getter				
			else 
				System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Must not be here script -{0:X}", ( DWORD )m_script ) );
			break;
		case BUSH_SCRIPT::LOCK_LOCK:
		case BUSH_SCRIPT::LOCK_UNLOCK:
		case BUSH_SCRIPT::RELAY_ON:
		case BUSH_SCRIPT::RELAY_OFF:
			if ( m_waitForOpcode == OPCODE::STATE_INFO )
				fnLockRelayScript( SCRIPT_STEP::SECOND_STEP );
			else if ( m_waitForOpcode == OPCODE::STATE_CHANGE )
				fnLockRelayScript( SCRIPT_STEP::SECOND_STEP );
			else 
				System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "Unpredicted behavior! Input processing, LOCK/UNLOKC script, unwaited OPCODE {0:X}", returnedOpcode ) );
			break;
		case BUSH_SCRIPT::GET_TEMPRETURE:
			m_pDataITC->SetData( bushState, bushStatus ); //TODO add temp check to set status overheat
			fnDefaultWait();
			break;		
		default:
			System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "ERROR! Check, no processing for this script name!" ) );
		}
	}
	else
	{
		if ( returnedOpcode == OPCODE::ALERT_BISH_BRISH ||
			 returnedOpcode == OPCODE::ALERT_TEMP_SENS ||
			 returnedOpcode == OPCODE::ALERT_TEMP_OVERHEAT )
			//status changed in data parsing
			m_pDataITC->SetData( bushStatus );
		else if ( returnedOpcode == OPCODE::STATE_CHANGE )
			m_pDataITC->SetData( bushState );
		else if ( m_waitForOpcode )
			; //waiting for another opcode, do nothing and wait for opcode or timer to send repeat of command 
		else
			System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "Unpredicted behavior! Input processing, no wait for opcode, not predicted opcode {0:X}", returnedOpcode ) );
	}
	
	return ERROR_SUCCESS;
}

DWORD BushInOutInterpretator::fnTimerWaitHandle()
{
	if ( m_waitForOpcode == OPCODE::NOT_VALUE )
		fnAskState();
	else if ( m_waitForOpcode == OPCODE::CONNECT_FINE )
		fnConnectCheck();
	else
	{
		if ( !m_bRepeatErr )
		{
			switch ( m_waitForOpcode )
			{
			case OPCODE::STATE_INFO:
				fnAskStateInit();
				break;
			case OPCODE::STATE_CHANGE:
				fnLockRelayScript( SCRIPT_STEP::SECOND_STEP );
				break;
			case OPCODE::TEMP_SENS_AVERAGE:
				fnGetHeatSens();
				break;
			default:
				System::Diagnostics::Debug::Assert( FALSE, System::String::Format( "Unpredicted behavior! Timeout processing, unwaited opcode -{0:X}", ( DWORD )m_waitForOpcode ) );
			}			
			m_bRepeatErr = TRUE;
		}
		else
			fnConnectCheck();
	}
	
	return ERROR_SUCCESS;
}

DWORD WINAPI fnMainIOBushThread( LPVOID lpParam )
{
	BushInOutInterpretator BushConnect( ( ( LPINTHREADDATA )lpParam )->pBushData, ( ( LPINTHREADDATA )lpParam )->pPortName );
	BushConnect.fnStart();

	while ( BushConnect.fnWaitForNextIO() );

	BushConnect.fnFinish();
	return ERROR_SUCCESS;
}
#include "BushData.h"


BOOL BushData::fnIsDataChanged() const
{
	DWORD fSuccess = WaitForSingleObject( m_hInfoChanged, NULL );

	return ( fSuccess == WAIT_OBJECT_0 ) ? TRUE : FALSE; // add options for another returns
}

DWORD BushData::fnSetData( const DATABUSH& dataToSave )
{
	DWORD fSuccess = WaitForSingleObject( m_hInfoMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnSetData: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	m_bushState = dataToSave;
	SetEvent( m_hInfoChanged );

	ReleaseMutex( m_hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD BushData::fnSetData( const BUSH_STATUS & statusToSave )
{
	DWORD fSuccess = WaitForSingleObject( m_hInfoMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnSetData: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	if ( m_status != statusToSave )
	{
		m_status = statusToSave;
		SetEvent( m_hInfoChanged );
	}

	ReleaseMutex( m_hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD BushData::fnSetData( const DATABUSH& dataToSave, const BUSH_STATUS& statusToSave )
{
	DWORD fSuccess = WaitForSingleObject( m_hInfoMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnSetData: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	m_status = statusToSave;
	m_bushState = dataToSave;
	SetEvent( m_hInfoChanged );

	ReleaseMutex( m_hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD BushData::fnGetData( DATABUSH& dataReturn, BUSH_STATUS& statusReturn ) const
{
	DWORD fSuccess = WaitForSingleObject( m_hInfoMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnGetData: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	dataReturn = m_bushState;
	statusReturn = m_status;
	ResetEvent( m_hInfoChanged );

	ReleaseMutex( m_hInfoMutex );

	return ERROR_SUCCESS;
}

//TODO rewrite using class with templates and checkung for wrong commands
DWORD BushData::fnSetCommand( const BUSH_SCRIPT & command )
{
	DWORD fSuccess = WaitForSingleObject( m_hCommandMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnSetCommand: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	m_command = command;
	SetEvent( m_hCommandEvent );

	ReleaseMutex( m_hCommandMutex );
	return ERROR_SUCCESS;
}

DWORD BushData::fnGetCommand( BUSH_SCRIPT & command ) const
{
	DWORD fSuccess = WaitForSingleObject( m_hCommandMutex, INFINITE ); // TODO add options for time waiting error
	
	if ( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED )
	{
		System::Diagnostics::Trace::TraceWarning( System::String::Format( "fnGetCommand: Mutex wait wrong return! {0:X}", fSuccess ) );
		return ERROR_DATA_NOT_ACCEPTED;
	}

	command = m_command;
	ResetEvent( m_hCommandEvent );

	ReleaseMutex( m_hCommandMutex );
	return ERROR_SUCCESS;
}




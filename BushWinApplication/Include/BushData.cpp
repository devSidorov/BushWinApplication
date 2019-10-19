#include "BushData.h"


BOOL const BushData::IsDataChanged()
{
	DWORD fSuccess = WaitForSingleObject( hInfoChanged, NULL );

	return ( fSuccess == WAIT_OBJECT_0 ) ? TRUE : FALSE; // add options for another returns
}

DWORD BushData::SetData( const DATABUSH& dataToSave )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error
	System::Diagnostics::Debug::Assert( fSuccess == WAIT_OBJECT_0 || fSuccess == WAIT_ABANDONED, System::String::Format( "ERROR! Mutex wait wrong return! {0:X}", fSuccess ) );
	
	bushState = dataToSave;
	SetEvent( hInfoChanged );

	ReleaseMutex( hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD BushData::SetData( const BUSH_STATUS & statusToSave )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error
	System::Diagnostics::Debug::Assert( fSuccess == WAIT_OBJECT_0 || fSuccess == WAIT_ABANDONED, System::String::Format( "ERROR! Mutex wait wrong return! {0:X}", fSuccess ) );

	if ( status != statusToSave )
	{
		status = statusToSave;
		SetEvent( hInfoChanged );
	}

	ReleaseMutex( hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD BushData::SetData( const DATABUSH& dataToSave, const BUSH_STATUS& statusToSave )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error
	System::Diagnostics::Debug::Assert( fSuccess == WAIT_OBJECT_0 || fSuccess == WAIT_ABANDONED, System::String::Format( "ERROR! Mutex wait wrong return! {0:X}", fSuccess ) );

	status = statusToSave;
	bushState = dataToSave;
	SetEvent( hInfoChanged );

	ReleaseMutex( hInfoMutex );

	return ERROR_SUCCESS;
}

DWORD const BushData::GetData( DATABUSH& dataReturn, BUSH_STATUS& statusReturn )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error
	System::Diagnostics::Debug::Assert( fSuccess == WAIT_OBJECT_0 || fSuccess == WAIT_ABANDONED, System::String::Format( "ERROR! Mutex wait wrong return! {0:X}", fSuccess ) );

	dataReturn = bushState;
	statusReturn = status;
	ResetEvent( hInfoChanged );

	ReleaseMutex( hInfoMutex );

	return ERROR_SUCCESS;
}


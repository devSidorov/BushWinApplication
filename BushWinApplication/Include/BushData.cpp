#include "BushData.h"


BOOL BushData::IsDataChanged()
{
	DWORD fSuccess = WaitForSingleObject( hInfoChanged, NULL );
	
	return ( fSuccess == WAIT_OBJECT_0 ) ? TRUE : FALSE; // add options for another returns
}

DWORD BushData::SetData( DATABUSH& dataToSave )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error

	System::Diagnostics::Debug::Assert( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED );
	
	bushState = dataToSave;
	SetEvent( hInfoChanged );

	ReleaseMutex( hInfoMutex );

	return 0;
}

DWORD BushData::GetData( DATABUSH& dataReturn )
{
	DWORD fSuccess = WaitForSingleObject( hInfoMutex, INFINITE ); // TODO add options for time waiting error

	System::Diagnostics::Debug::Assert( fSuccess != WAIT_OBJECT_0 && fSuccess != WAIT_ABANDONED );

	dataReturn = bushState;
	ResetEvent( hInfoChanged );

	ReleaseMutex( hInfoMutex );

	return 0;
}

#pragma once

#include <windows.h>

#include "SerialPortBush.h"

class BushData
{
private:
	DATABUSH m_bushState;
	BUSH_STATUS m_status;	
	BUSH_SCRIPT m_command;
	
	HANDLE m_hInfoChanged;
	HANDLE m_hInfoMutex;

	HANDLE m_hCommandEvent;
	HANDLE m_hCommandMutex;

	HANDLE m_hDaugtherThread;
public:

	BushData() {
		ClearMemory();
		m_hInfoChanged = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_hCommandEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_hInfoMutex = CreateMutex( NULL, FALSE, NULL );
		m_hCommandMutex = CreateMutex( NULL, FALSE, NULL );
		m_hDaugtherThread = nullptr;
	}

	~BushData() {
		CloseHandle( m_hInfoChanged );
		CloseHandle( m_hInfoMutex );
		CloseHandle( m_hCommandEvent );
		CloseHandle( m_hCommandMutex );
	}

	void ClearMemory() {
		SecureZeroMemory( &m_bushState, sizeof( DATABUSH ) );
		m_status = BUSH_STATUS::NO_STATUS;
		m_command = BUSH_SCRIPT::NO_SCRIPT;
		m_hDaugtherThread = nullptr;
		return;
	}

	BOOL fnIsDataChanged() const;
	HANDLE fnGetCommandEvent() const {
		return m_hCommandEvent;
	}
	
	DWORD fnSetData( const DATABUSH& dataToSave );
	DWORD fnSetData( const BUSH_STATUS& statusToSave );
	DWORD fnSetData( const DATABUSH& dataToSave, const BUSH_STATUS & statusToSave );

	DWORD fnGetData( DATABUSH& dataReturn, BUSH_STATUS& statusReturn ) const;

	DWORD fnSetCommand( const BUSH_SCRIPT& command );
	DWORD fnGetCommand( BUSH_SCRIPT& command ) const;

	HANDLE fnGetDaughterHandle() const {
		return m_hDaugtherThread;
	}
	void SetDaughterHandle( const HANDLE hThread ) {
		m_hDaugtherThread = hThread;
		return;
	}

};


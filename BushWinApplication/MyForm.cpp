#include "MyForm.h"
#include "Include/BushInOutInterpretator.h"

#include <array>
#include <msclr/marshal.h>

using namespace System;
using namespace System::Windows::Forms;

BushData g_ITCdataBush;
HANDLE g_hIObushThread = nullptr;
INTHREADDATA g_dataToThread;

[STAThreadAttribute]
void Main( array<String^>^ args ) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault( false );
	BushWinApplication::MyForm form;
	Application::Run( %form );
}

Int32 BushWinApplication::MyForm::fnStartBushIOThread( String^ pPortName )
{		
	// types change for use low functions
	msclr::interop::marshal_context context;
	g_ITCdataBush.ClearMemory();
	g_dataToThread.ClearMemory();
	_tcscpy_s<MAX_PATH>( g_dataToThread.acPortName, context.marshal_as<const TCHAR*>( pPortName ) );
	g_dataToThread.pBushData = &g_ITCdataBush;
	
	g_hIObushThread = CreateThread( nullptr, NULL, fnMainIOBushThread, &g_dataToThread, NULL, nullptr );
	if ( !g_hIObushThread )
	{
		System::Diagnostics::Trace::TraceWarning( "IO thread start fail" );
		return ERROR_THREAD_NOT_IN_PROCESS;
	}
	
	timerCheckData->Enabled = TRUE;
	return ERROR_SUCCESS;
}

Void BushWinApplication::MyForm::fnCloseOldBushIOThread()
{
	g_ITCdataBush.fnSetCommand( BUSH_SCRIPT::DISCONNECT );
	DWORD fSuccess = WaitForSingleObject( g_hIObushThread, 5000 );
	if ( fSuccess != WAIT_OBJECT_0 )
	{
		System::Diagnostics::Trace::TraceWarning( "IO thread normal finish failed!" );
		TerminateThread( g_hIObushThread, ( DWORD )ERROR_HANDLE_NO_LONGER_VALID );
		TerminateThread( g_ITCdataBush.fnGetDaughterHandle(), ( DWORD )ERROR_HANDLE_NO_LONGER_VALID );
	}
	
	return;
}

Void BushWinApplication::MyForm::fnFormGuiEnable( bool isTRUE )
{
	comBoxPortNames->Enabled = isTRUE;
}

Void BushWinApplication::MyForm::fnInfoLabelsReset()
{
	labelBushConnect->Text = L"";
	labelBushDoor->Text = L"";
	labelBushLock->Text = L"";
	labelBushSens->Text = L"";
	labelBushRelay->Text = L"";	
}

Void BushWinApplication::MyForm::fnLockUnlockDoor()
{
	g_ITCdataBush.fnSetCommand( ( m_isLockLocked ) ? BUSH_SCRIPT::LOCK_UNLOCK : BUSH_SCRIPT::LOCK_LOCK );	
	return;
}

Void BushWinApplication::MyForm::fnStatusLabelUpdate( Int16 bushStatus )
{
	switch ( bushStatus )
	{		
	case BUSH_STATUS::NO_STATUS:
		labelBushConnect->Text = L"Выберите порт!";
		break;
	case BUSH_STATUS::DISCONNECTED:
		labelBushConnect->Text = L"Не подкючен! Ожидание ответа...";
		break;
	case BUSH_STATUS::CONNECTED:
		labelBushConnect->Text = L"Подключен";
		break;
	case BUSH_STATUS::HEAT_SENS_ERR:
		labelBushConnect->Text = L"Ошибка датчика температуры!";
		break;
	case BUSH_STATUS::BUSH_BRISH_ERR:
		labelBushConnect->Text = L"Ошибка БУШ/БРИШ!";
		break;
	case BUSH_STATUS::OVERHEATED:
		labelBushConnect->Text = L"Превышена критическая температура!";
		break;
	default:
		System::Diagnostics::Debug::WriteLine( "WARNING! No such bush status in StatusLabelUpdate" );
		break;
	};
}

Void BushWinApplication::MyForm::fnTrayMenuUpdate( Int16 bushStatus, Boolean bIsLockLocked )
{
	if ( !m_isDoorClosed )
	{
		trayMenuItemDoor->Text = "";
		trayMenuItemDoor->Visible = FALSE;
		return;
	}
	
	switch ( bushStatus )
	{
	case BUSH_STATUS::NO_STATUS:
	case BUSH_STATUS::DISCONNECTED:
		trayMenuItemDoor->Text = "";
		trayMenuItemDoor->Visible = FALSE;
		break;	
	case BUSH_STATUS::CONNECTED:
	case BUSH_STATUS::HEAT_SENS_ERR:
	case BUSH_STATUS::BUSH_BRISH_ERR:
	case BUSH_STATUS::OVERHEATED:
		trayMenuItemDoor->Text = ( ( bIsLockLocked ) ? L"Открыть" : L"Закрыть" ) + L" замок";
		trayMenuItemDoor->Visible = TRUE;
		break;
	default:
		System::Diagnostics::Debug::WriteLine( "WARNING! No such bush status in trayMenuUpdate" );
		break;
	};
}

Int32 BushWinApplication::MyForm::fnOnTimerUpdate()
{
	BUSH_STATUS bushStatus;
	DATABUSH bushState;

	//TODO check thread is runnig
	if ( g_ITCdataBush.fnIsDataChanged() )
	{
		g_ITCdataBush.fnGetData( bushState, bushStatus );
		
		m_isDoorClosed = ( bushState.info[INFO_BYTE_BITS::DOOR] != 0 );
		m_isLockLocked = ( bushState.info[INFO_BYTE_BITS::LOCK] != 0 );
		m_isRelayOn = ( bushState.info[INFO_BYTE_BITS::RELAY] != 0 );
		
		fnInfoLabelsReset();
		fnStatusLabelUpdate( bushStatus );
		fnTrayMenuUpdate( bushStatus, m_isLockLocked );
		fnTrayIconUpdate( bushStatus );

		if ( bushStatus != NO_STATUS && bushStatus != DISCONNECTED )
		{
			labelBushDoor->Text = m_isDoorClosed ? L"Закрыта" : L"Открыта";
			labelBushLock->Text = m_isLockLocked ? L"Закрыт" : L"Открыт";
			labelBushSens->Text = bushState.averageTemp.ToString();
			labelBushRelay->Text = m_isRelayOn ? L"Включено" : L"Выключено";
		}
	}

	return ERROR_SUCCESS;
}

Void BushWinApplication::MyForm::fnTrayIconUpdate( Int16 bushStatus )
{
	switch ( bushStatus )
	{
	case BUSH_STATUS::NO_STATUS:
	case BUSH_STATUS::DISCONNECTED:
		trayNotification->Icon = m_pIcoDisconnect;
		break;
	case BUSH_STATUS::CONNECTED:
	case BUSH_STATUS::HEAT_SENS_ERR:
	case BUSH_STATUS::BUSH_BRISH_ERR:
		if ( !m_isDoorClosed )
			trayNotification->Icon = m_pIcoOpen;
		else if ( m_isLockLocked )
			trayNotification->Icon = m_pIcoLock;
		else
			trayNotification->Icon = m_pIcoClose;
		break;
	case BUSH_STATUS::OVERHEATED:
		trayNotification->Icon = m_pIcoOverHeat;
		break;
	default:
		System::Diagnostics::Debug::WriteLine( "WARNING! No such bush status in fnTrayIconUpdate" );
		break;
	};
}


//func timer	
//	check thread is runnig
//	check if info changed
//		get info to form

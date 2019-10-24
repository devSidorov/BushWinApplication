#include "MyForm.h"
#include "Include/BushInOutInterpretator.h"

#include <array>
#include <msclr/marshal.h>

using namespace System;
using namespace System::Windows::Forms;

BushData ITCdataBush;
HANDLE hIObushThread = nullptr;

[STAThreadAttribute]
void Main( array<String^>^ args ) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault( false );
	BushWinApplication::MyForm form;
	Application::Run( %form );
}

Int32 BushWinApplication::MyForm::BushIOThreadStart( String^ pPortName )
{
	INTHREADDATA dataToThread;
	// types change for use low functions
	msclr::interop::marshal_context context;
	dataToThread.pPortName = context.marshal_as<const TCHAR*>( pPortName );
	dataToThread.pBushData = &ITCdataBush;
	
	hIObushThread = CreateThread( nullptr, NULL, fnMainIOBushThread, &dataToThread, NULL, nullptr );
	System::Diagnostics::Debug::Assert( hIObushThread, "ERROR! IO thread hasn't started" ); //TODO add error check

	timerCheckData->Enabled = TRUE;
	
	return ERROR_SUCCESS;
}

Void BushWinApplication::MyForm::FormGuiEnable( bool isTRUE )
{
	comBoxPortNames->Enabled = isTRUE;
}

Void BushWinApplication::MyForm::InfoLabelsReset()
{
	labelBushConnect->Text = L"";
	labelBushDoor->Text = L"";
	labelBushLock->Text = L"";
	labelBushSens->Text = L"";
	labelBushRelay->Text = L"";	
}

Void BushWinApplication::MyForm::fnLockUnlockDoor()
{
	ITCdataBush.fnSetCommand( ( m_isLockLocked ) ? BUSH_SCRIPT::LOCK_UNLOCK : BUSH_SCRIPT::LOCK_LOCK );	
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
	if ( ITCdataBush.fnIsDataChanged() )
	{
		ITCdataBush.fnGetData( bushState, bushStatus );
		
		m_isDoorClosed = ( bushState.info[INFO_BYTE_BITS::DOOR] != 0 );
		m_isLockLocked = ( bushState.info[INFO_BYTE_BITS::LOCK] != 0 );
		m_isRelayOn = ( bushState.info[INFO_BYTE_BITS::RELAY] != 0 );
		
		InfoLabelsReset();
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

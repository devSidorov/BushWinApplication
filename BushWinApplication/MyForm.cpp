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

Void BushWinApplication::MyForm::fnOnStart()
{
	//adding log file
	Diagnostics::Trace::Listeners->Add( gcnew System::Diagnostics::TextWriterTraceListener( m_logFile ) );
	Diagnostics::Trace::AutoFlush = true;
	Diagnostics::Trace::TraceInformation( "Application started" ); //TODO add start time


	fnGetUserSettings();
	//form update
	Icon = m_pIcoDisconnect;
	ShowIcon = true;
	fnStatusLabelUpdate( 0 );
	fnTrayIconUpdate( 0 );

	return;
}

Void BushWinApplication::MyForm::fnGetUserSettings()
{
	m_ConfigXmlFile = gcnew Xml::XmlDocument;

	Xml::XmlTextReader^ configFileRead = gcnew Xml::XmlTextReader( "appUserConfig" );
	if ( configFileRead )
		m_ConfigXmlFile->Load( configFileRead );
	else //TODO add implementation if no settings file
		;//m_ConfigXmlFile->Load( L"<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n" +
		//					   L"<settings showDoorState = \"true\" showOverHeat = \"true\" portName = \"\">\n" +
		//					   L"</settings>" );

	configFileRead->Close();
	String^ compareString = "true";
	chBoxLockDoor->Checked = compareString == m_ConfigXmlFile->DocumentElement->GetAttribute( "showDoorState" );
	chBoxOverheat->Checked = compareString == m_ConfigXmlFile->DocumentElement->GetAttribute( "showOverHeat" );
	compareString = m_ConfigXmlFile->DocumentElement->GetAttribute( "portName" );
	//TODO add implementation for port name

	return;
}

Void BushWinApplication::MyForm::fnSetUserSettings()
{
	m_ConfigXmlFile->DocumentElement->SetAttribute( "showDoorState", ( chBoxLockDoor->Checked == true ) ? "true" : "false" );
	m_ConfigXmlFile->DocumentElement->SetAttribute( "showOverHeat", ( chBoxOverheat->Checked == true ) ? "true" : "false" );
	//TODO add port name save

	m_ConfigXmlFile->Save( "appUserConfig" );

	return;
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
		System::Diagnostics::Trace::TraceWarning( "fnStatusLabelUpdate: No such bush status in StatusLabelUpdate" );
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
		System::Diagnostics::Trace::TraceWarning( "fnTrayMenuUpdate: No such bush status in trayMenuUpdate" );
		break;
	};
}

Int32 BushWinApplication::MyForm::fnOnTimerUpdate()
{
	BUSH_STATUS bushStatus;
	DATABUSH bushState;

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
		trayNotification->Icon = m_pIcoOverHeat; //TODO add changing every second tray notification
		break;
	default:
		System::Diagnostics::Trace::TraceWarning( "fnTrayIconUpdate: No such bush status in fnTrayIconUpdate" );
		break;
	};
}


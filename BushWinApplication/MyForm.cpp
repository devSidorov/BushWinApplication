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

short BushWinApplication::MyForm::fnOnStart()
{
	Icon = m_pIcoDisconnect;
	ShowIcon = true;
	fnStatusLabelUpdate( 0 );
	fnTrayIconUpdate( 0 );
	
	String^ pPathLocalData = Environment::GetFolderPath( Environment::SpecialFolder::CommonApplicationData );
	
	if ( String::IsNullOrEmpty( pPathLocalData ) )
	{
		System::Diagnostics::Trace::TraceError( "fnOnStart: Can't get path to CommonApplicationData!" );
		return -1;
	}

	pPathLocalData += L"\\EdelveisBush";
	if ( ! IO::Directory::Exists( pPathLocalData ) )
		IO::Directory::CreateDirectory( pPathLocalData );
	pPathLocalData += L"\\";
	
	//adding log file
	String^ pLogFileName = gcnew String( L"bush_runtime.log" );
	if ( !IO::File::Exists( pPathLocalData + pLogFileName ) )
		m_pLogFile = IO::File::Create( pPathLocalData + pLogFileName );
	else
		m_pLogFile = gcnew IO::FileStream( pPathLocalData + pLogFileName, IO::FileMode::Append );
	
	Diagnostics::Trace::Listeners->Add( gcnew System::Diagnostics::TextWriterTraceListener( m_pLogFile ) );
	Diagnostics::Trace::AutoFlush = true;
	Diagnostics::Trace::TraceInformation( String::Format ( "+++++++++| Application started: {0} |+++++++++", DateTime::Now ) ); 
	
	fnGetUserSettings( pPathLocalData );
	
	return 0;
}

short BushWinApplication::MyForm::fnGetUserSettings( String^ pPathLocalData )
{
	String^ pXmlConfigFileName = gcnew String( L"appUserConfig" );
	m_pConfigXmlFile = gcnew Xml::XmlDocument;
	m_pXmlConfigFilePath = pPathLocalData + pXmlConfigFileName;

	//if file exist take settings from it, if not from default file
	Xml::XmlTextReader^ configFileRead = gcnew Xml::XmlTextReader( ( IO::File::Exists( pPathLocalData + pXmlConfigFileName ) ) ? m_pXmlConfigFilePath : pXmlConfigFileName );
	m_pConfigXmlFile->Load( configFileRead );
	configFileRead->Close();	

	String^ compareString = "true";
	chBoxLockDoor->Checked = compareString == m_pConfigXmlFile->DocumentElement->GetAttribute( "showDoorState" );
	chBoxOverheat->Checked = compareString == m_pConfigXmlFile->DocumentElement->GetAttribute( "showOverHeat" );
	compareString = m_pConfigXmlFile->DocumentElement->GetAttribute( "portName" );
	//TODO add implementation for port name

	return 0;
}

short BushWinApplication::MyForm::fnSetUserSettings()
{
	m_pConfigXmlFile->DocumentElement->SetAttribute( "showDoorState", ( chBoxLockDoor->Checked == true ) ? "true" : "false" );
	m_pConfigXmlFile->DocumentElement->SetAttribute( "showOverHeat", ( chBoxOverheat->Checked == true ) ? "true" : "false" );
	//TODO add port name save

	m_pConfigXmlFile->Save( m_pXmlConfigFilePath );

	return 0;
}

short BushWinApplication::MyForm::fnConnectToPort( String ^ pPortName )
{
	static bool bNotFirstChoose = false;

	fnFormGuiEnable( false );
	if ( bNotFirstChoose )
	{
		fnCloseOldBushIOThread();
		fnInfoLabelsReset();
		bNotFirstChoose = false;
	}

	Int32 fSuccess = fnStartBushIOThread( pPortName );
	if ( !fSuccess )
		bNotFirstChoose = true;

	fnFormGuiEnable( true );
	return 0;
}

short BushWinApplication::MyForm::fnStartBushIOThread( String^ pPortName )
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
	return 0;
}

short BushWinApplication::MyForm::fnCloseOldBushIOThread()
{
	g_ITCdataBush.fnSetCommand( BUSH_SCRIPT::DISCONNECT );
	DWORD fSuccess = WaitForSingleObject( g_hIObushThread, 5000 );
	if ( fSuccess != WAIT_OBJECT_0 )
	{
		System::Diagnostics::Trace::TraceWarning( "IO thread normal finish failed!" );
		TerminateThread( g_hIObushThread, ( short )ERROR_HANDLE_NO_LONGER_VALID );
		TerminateThread( g_ITCdataBush.fnGetDaughterHandle(), ( short )ERROR_HANDLE_NO_LONGER_VALID );
	}
	
	return 0;
}

short BushWinApplication::MyForm::fnFormGuiEnable( bool isTRUE )
{
	comBoxPortNames->Enabled = isTRUE;
	return 0;
}

short BushWinApplication::MyForm::fnInfoLabelsReset()
{
	labelBushConnect->Text = L"";
	labelBushDoor->Text = L"";
	labelBushLock->Text = L"";
	labelBushSens->Text = L"";
	labelBushRelay->Text = L"";	

	return 0;
}

short BushWinApplication::MyForm::fnLockUnlockDoor()
{
	g_ITCdataBush.fnSetCommand( ( m_isLockLocked ) ? BUSH_SCRIPT::LOCK_UNLOCK : BUSH_SCRIPT::LOCK_LOCK );	
	return 0;
}

short BushWinApplication::MyForm::ReNew_ComPorts()
{
	array<String^>^ serialPorts = nullptr;

	comBoxPortNames->Items->Clear();
	serialPorts = IO::Ports::SerialPort::GetPortNames();
	if ( serialPorts )
	{
		Array::Sort( serialPorts );
		comBoxPortNames->Items->AddRange( serialPorts );
	}
	return 0;
}

short BushWinApplication::MyForm::fnStatusLabelUpdate( Int16 bushStatus )
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
	return 0;
}

short BushWinApplication::MyForm::fnTrayMenuUpdate( Int16 bushStatus, Boolean bIsLockLocked )
{
	if ( !m_isDoorClosed )
	{
		trayMenuItemDoor->Text = "";
		trayMenuItemDoor->Visible = FALSE;
		return 0;
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
	return 0;
}

short BushWinApplication::MyForm::fnTrayIconUpdate( Int16 bushStatus )
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
	return 0;
}

short BushWinApplication::MyForm::fnFormAppear()
{
	WindowState = FormWindowState::Normal;
	TopMost = true;
	TopMost = false;
	return 0;
}

short BushWinApplication::MyForm::fnOnTimerUpdate()
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

	return 0;
}

//TODO add IPC
//TODO notification blink
//TODO change icon then port change
//TODO check window change when port drop down

//TODO add implementation for notification checkbox
//TODO add to trace information: port name and IO proc thread start stop


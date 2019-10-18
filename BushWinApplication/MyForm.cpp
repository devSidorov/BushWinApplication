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

Void BushWinApplication::MyForm::StatusLabelUpdate( const Int32& bushStatus )
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
		System::Diagnostics::Debug::Assert( hIObushThread, "ERROR! No such bish status" );
		break;
	};
}

Int32 BushWinApplication::MyForm::OnTimerUpdate()
{
	BUSH_STATUS bushStatus;
	DATABUSH bushState;

	//TODO check thread is runnig
	if ( ITCdataBush.IsDataChanged() )
	{
		ITCdataBush.GetData( bushState, bushStatus );
		InfoLabelsReset();
		StatusLabelUpdate( bushStatus );
		if ( bushStatus != NO_STATUS && bushStatus != DISCONNECTED )
		{
			labelBushDoor->Text = ( bushState.info[INFO_BYTE_BITS::DOOR]==0 ) ? L"Открыта" : L"Закрыта";
			labelBushLock->Text = ( bushState.info[INFO_BYTE_BITS::LOCK]==0 ) ? L"Открыт" : L"Закрыт";
			labelBushRelay->Text = ( bushState.info[INFO_BYTE_BITS::RELAY]==0 ) ? L"Выключено" : L"Включено";			
		}


	}

	return ERROR_SUCCESS;
}


//func timer	
//	check thread is runnig
//	check if info changed
//		get info to form

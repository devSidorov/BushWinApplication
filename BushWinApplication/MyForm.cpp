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

typedef struct InThreadData
{
	const TCHAR* pPortName;
	BushData* pBushData;
} INTHREADDATA, *LPINTHREADDATA;

DWORD WINAPI MainIOBushThread( LPVOID lpParam )
{
	BushInOutInterpretator BushConnect( ( ( LPINTHREADDATA )lpParam )->pBushData, ( ( LPINTHREADDATA )lpParam )->pPortName );
	BushConnect.Start();

	while ( BushConnect.WaitForNextIO() );

	BushConnect.Finish();
	return ERROR_SUCCESS;
}

Int32 BushWinApplication::MyForm::BushIOThreadStart( String^ pPortName )
{
	INTHREADDATA dataToThread;
	// types change for use low functions
	msclr::interop::marshal_context context;
	dataToThread.pPortName = context.marshal_as<const TCHAR*>( pPortName );
	dataToThread.pBushData = &ITCdataBush;
	
	hIObushThread = CreateThread( nullptr, NULL, MainIOBushThread, &dataToThread, NULL, nullptr );
	System::Diagnostics::Debug::Assert( hIObushThread, "ERROR! IO thread hasn't started" ); //TODO add error check

	timerCheckData->Enabled = TRUE;
	
	return ERROR_SUCCESS;
}

Void BushWinApplication::MyForm::FormGuiEnable( bool isTRUE )
{
	comBoxPortNames->Enabled = isTRUE;
}

Int32 BushWinApplication::MyForm::OnTimerUpdate()
{
	BUSH_STATUS bushStatus;
	DATABUSH bushState;

	//TODO check thread is runnig
	if ( ITCdataBush.IsDataChanged() )
	{
		ITCdataBush.GetData( bushState, bushStatus );
		labelBushDoor->Text = ( bushState.info[INFO_BYTE_BITS::DOOR] == 0 ) ? L"Закрыта" : L"Открыта";
		labelBushLock->Text = ( bushState.info[INFO_BYTE_BITS::LOCK] == 0 ) ? L"Закрыт" : L"Открыт";

	}

	return ERROR_SUCCESS;
}


//func timer	
//	check thread is runnig
//	check if info changed
//		get info to form

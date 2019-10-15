#include "MyForm.h"
#include "Include/ThreadBushIO.h"

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

	hIObushThread = CreateThread( nullptr, NULL, MainIOBushThread, &dataToThread, NULL, nullptr );
	System::Diagnostics::Debug::Assert( hIObushThread, "ERROR! IO thread hasn't started" ); //TODO add error check
	
	return ERROR_SUCCESS;
}

Void BushWinApplication::MyForm::FormGuiEnable( bool isTRUE )
{
	comBoxPortNames->Enabled = isTRUE;
}

//func timer	
//	check thread is runnig
//	check if info changed
//		get info to form

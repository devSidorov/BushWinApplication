#include "MyForm.h"

#include "..\BushWinApplication\Include\SerialPortBush.h"
#include "..\BushWinApplication\Include\BushData.h"
#include <array>
#include <msclr/marshal.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main( array<String^>^ args ) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault( false );
	BushWinApplication::MyForm form;
	Application::Run( %form );
}

Int32 BushWinApplication::MyForm::BushConnect( String^ pPortName )
{
	// types change for use low functions
	msclr::interop::marshal_context context;
	const TCHAR* pName = context.marshal_as<const TCHAR*>( pPortName );

	SerialPortBush bush( pName );
	bush.Open();
	bush.Write( 0x04, 0x00 );
	bush.Read();

	return 0;
}

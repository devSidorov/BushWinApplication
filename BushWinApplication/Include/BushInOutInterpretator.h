#pragma once
#include "SerialPortBush.h"
class BushInOutInterpretator : public SerialPortBush
{
private:
	enum EVENT_ARR {
		COMMAND,
		BUSH_INPUT,
		EV_COUNT
	};

	BushData* pDataITC;
	HANDLE haEvHandler[EV_COUNT];
	BYTE waitForOpcode;
	DWORD dWaitTime;

private:
	VOID SecureClassMemory() {
		pDataITC = nullptr;
		SecureZeroMemory( haEvHandler, sizeof( HANDLE )*EV_COUNT );
		waitForOpcode = OPCODE::NOT_VALUE;
		dWaitTime = INFINITE;
	}

public:
	BushInOutInterpretator( BushData* pDataToITC, const TCHAR* pcBushPortName ) : SerialPortBush( pcBushPortName ) {
		SecureClassMemory();
		pDataITC = pDataToITC;
		haEvHandler[EVENT_ARR::COMMAND] = pDataITC->GetCommandEvent();
	}
	~BushInOutInterpretator() { 
		for ( int i = 0; i < EV_COUNT; i++ )
			CloseHandle( haEvHandler[i] );
		pDataITC = nullptr;
	}

	DWORD Start();
	DWORD Finish();

	BOOL WaitForNextIO();
	DWORD InputBushHandle();	
};

DWORD WINAPI FromBushThread( LPVOID lpParam );


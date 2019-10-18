#pragma once
#include "SerialPortBush.h"
#include "BushData.h"

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
	BUSH_SCRIPT script;
	DWORD dWaitTime;
	DWORD dWaitIterator;

private:
	VOID SecureClassMemory() {
		pDataITC = nullptr;
		SecureZeroMemory( haEvHandler, sizeof( HANDLE )*EV_COUNT );
		waitForOpcode = OPCODE::NOT_VALUE;
		script = BUSH_SCRIPT::NO_SCRIPT;
		dWaitTime = INFINITE;
		dWaitIterator = 0;

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

private:
	DWORD DefaultWait();
	DWORD ConnectCheck();
	DWORD AskState();
};



DWORD WINAPI FromBushThread( LPVOID lpParam );
typedef struct InThreadData
{
	const TCHAR* pPortName;
	BushData* pBushData;
} INTHREADDATA, *LPINTHREADDATA;

DWORD WINAPI MainIOBushThread( LPVOID lpParam );

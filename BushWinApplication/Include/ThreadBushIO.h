#pragma once

#include <windows.h>
#include "BushData.h"
#include "SerialPortBush.h"

typedef struct InThreadData
{
	const TCHAR* pPortName;
	BushData* pBushData;
} INTHREADDATA, *LPINTHREADDATA;

DWORD WINAPI MainIOBushThread( LPVOID lpParam );


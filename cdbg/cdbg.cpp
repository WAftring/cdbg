// cdbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <DbgEng.h>
#include <stdio.h>

#pragma comment(lib, "dbgeng.lib")
#define VERSION 1
#define FLAGS 0

#define DBG_SUCCESS(func) \
hr = func; \
if (!SUCCEEDED(hr)) \
	printf("Dbg invocation failed with error: %x\n", hr); \
	goto Exit;\
 }\

// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-debugconnectwide
HRESULT CdbgInit(PULONG Version, PULONG Flags);
HRESULT CdbgFunc1(PDEBUG_CLIENT Client, PCSTR Args);
int main()
{
	IDebugClient* dbgClient = NULL;
	IDebugInputCallbacks* dbgInput = NULL;
	IDebugOutputCallbacks* dbgOutput = NULL;
	IDebugEventCallbacks* dbgCallbacks = NULL;
	IDebugControl3* dbgCtrl = NULL;
	ULONG dt = 0;
	HRESULT hr;
	hr = DebugCreate(__uuidof(IDebugClient), (void**)&dbgClient);
	if (!SUCCEEDED(hr))
	{
		printf("DebugCreate failed with error: %x\n", hr);
		goto Exit;
	}
	dbgClient->ConnectSession(0, 500);
	dbgClient->GetEventCallbacks(&dbgCallbacks);
	dbgClient->GetInputCallbacks(&dbgInput);
	dbgClient->GetOutputCallbacks(&dbgOutput);
	dbgClient->QueryInterface(__uuidof(IDebugControl3), (void**)&dbgCtrl);
	dbgCtrl->SetExecutionStatus(DEBUG_STATUS_GO);
	dbgClient->OpenDumpFile("C:\\CaseData\\2204190040006874_Morgan\\2022-12-05\\lsass.exe.1696.dmp");
	dbgCtrl->WaitForEvent(0, INFINITE);
	dbgCtrl->GetCurrentTimeDate(&dt);

	printf("Date Time: %lu\n", dt);
Exit:
	
	return 0;
}

HRESULT CdbgInit(PULONG Version, PULONG Flags
)
{
	*Version = VERSION;
	*Flags = 0;
	return S_OK;
}

void CdbgUninit()
{
	
}

void CdbgNotify(ULONG Notify, ULONG64 Argument)
{
	
}

HRESULT CdbgFunc1(PDEBUG_CLIENT Client, PCSTR Args)
{
	return S_OK;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

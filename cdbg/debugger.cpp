#include "debugger.h"
#define LOG_IF_FAIL(hr, msg) \
if (!SUCCEEDED(hr)) \
{\
	printf(msg, hr); \
	goto Exit;\
 }\

typedef struct DbgEvent
{
	ULONG Type;
	ULONG PID;
	ULONG TID;
	PVOID ExtraInfo;
	ULONG ExtraInfoSize;
	ULONG ExtraInfoUsed;
	char Description[1024];
	const ULONG DescriptionSize = 1024;
	ULONG DescriptionUsed;
} DbgEvent;

DWORD WINAPI DbgMain(LPVOID lpParam)
{
	IDebugClient* dbgClient = NULL;
	IDebugInputCallbacks* dbgInput = NULL;
	IDebugOutputCallbacks* dbgOutput = NULL;
	IDebugEventCallbacks* dbgCallbacks = NULL;
	IDebugControl3* dbgCtrl = NULL;
	DbgEvent event;
	HRESULT hr;
	BOOL bRunning = TRUE;
	CdbgOut dbgOut;
	CdbgEvents dbgEvents;

	hr = DebugCreate(__uuidof(IDebugClient), (void**)&dbgClient);
	LOG_IF_FAIL(hr, "DebugCreate failed with: %x\n");
	hr = dbgClient->ConnectSession(0, 500);
	dbgClient->SetOutputMask(DEBUG_OUTPUT_NORMAL | DEBUG_OUTPUT_ERROR);
	dbgClient->SetOutputCallbacks(&dbgOut);
	LOG_IF_FAIL(hr, "dbgClient->ConnectSession failed with: %x\n");
	hr = dbgClient->GetEventCallbacks(&dbgCallbacks);
	LOG_IF_FAIL(hr, "dbgClient->GetEventCallbacks failed with: %x\n");
	hr = dbgClient->GetInputCallbacks(&dbgInput);
	LOG_IF_FAIL(hr, "dbgClient->GetInputCallbacks failed with: %x\n");
	dbgClient->QueryInterface(__uuidof(IDebugControl3), (void**)&dbgCtrl);
	LOG_IF_FAIL(hr, "dbgClient->QueryInterface failed with: %x\n");

	dbgCtrl->SetExecutionStatus(DEBUG_STATUS_GO);
	dbgClient->OpenDumpFile("C:\\Tools\\notepad.dmp");
	
	while (bRunning)
	{
		dbgCtrl->WaitForEvent(0, INFINITE);
	}

Exit:
	return 0;
}

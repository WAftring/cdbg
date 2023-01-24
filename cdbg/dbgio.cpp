#include "dbgio.hpp"
#include <DbgEng.h>

#include "CdbgOut.hpp"
#include "CdbgEvents.hpp"
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

IDebugClient* pDbgClient = NULL;
IDebugInputCallbacks* pDbgInput = NULL;
CdbgOut* pDbgOutput = NULL;
IDebugEventCallbacks* pDbgCallbacks = NULL;
IDebugControl3* pDbgCtrl = NULL;
bool g_Initialized; 

BOOL DbgioInit()
{
	HRESULT hr = S_OK;
	if (g_Initialized)
		return false;
	pDbgOutput = new CdbgOut();
	hr = DebugCreate(__uuidof(IDebugClient), (void**)&pDbgClient);
	LOG_IF_FAIL(hr, "DebugCreate failed with: %x\n");
	hr = pDbgClient->ConnectSession(0, 500);
	pDbgClient->SetOutputMask(DEBUG_OUTPUT_NORMAL | DEBUG_OUTPUT_ERROR);
	pDbgClient->SetOutputCallbacks(pDbgOutput);
	LOG_IF_FAIL(hr, "pDbgClient->ConnectSession failed with: %x\n");
	hr = pDbgClient->GetEventCallbacks(&pDbgCallbacks);
	LOG_IF_FAIL(hr, "pDbgClient->GetEventCallbacks failed with: %x\n");
	hr = pDbgClient->GetInputCallbacks(&pDbgInput);
	LOG_IF_FAIL(hr, "pDbgClient->GetInputCallbacks failed with: %x\n");
	pDbgClient->QueryInterface(__uuidof(IDebugControl3), (void**)&pDbgCtrl);
	LOG_IF_FAIL(hr, "pDbgClient->QueryInterface failed with: %x\n");

	pDbgCtrl->SetExecutionStatus(DEBUG_STATUS_GO);
	g_Initialized = true;
Exit:
	return S_OK == hr;
}

DWORD DbgioInvoke(const char* szCommand)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_CONTINUE;
	hr = pDbgCtrl->Execute(DEBUG_OUTCTL_THIS_CLIENT, szCommand, NULL);
	//LOG_IF_FAIL(hr, "Execute failed with error: %x\n");
	// TODO(will): Add some logic for determining a fatal HRESULT
Exit:
	
	return dwResult;
}

DWORD DbgioAttachDump(const char* szDumpPath)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_FAIL;
	if (!g_Initialized)
		return DBGIO_UNINITALIZED;

	hr = pDbgClient->OpenDumpFile(szDumpPath);
	LOG_IF_FAIL(hr, "OpenDumpFile failed with error: %x\n");
	hr = pDbgCtrl->WaitForEvent(0, INFINITE);
	LOG_IF_FAIL(hr, "WaitForEvent failed with error: %x\n");
	hr = S_OK;
Exit:
	if (!SUCCEEDED(hr))
		dwResult = DBGIO_FAIL;
	SetLastError(hr);
	return dwResult;
}
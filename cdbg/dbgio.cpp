#include "dbgio.hpp"
#include <DbgEng.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

#include "CdbgOut.hpp"
#include "CdbgEvents.hpp"

typedef struct DbgEvent
{
	ULONG Type;
	ULONG PID;
	ULONG TID;
	PVOID ExtraInfo;
	ULONG ExtraInfoSize;
	ULONG ExtraInfoUsed;
	WCHAR Description[1024];
	const ULONG DescriptionSize = 1024;
	ULONG DescriptionUsed;
} DbgEvent;

IDebugClient8* pDbgClient = NULL;
IDebugInputCallbacks* pDbgInput = NULL;
CdbgOut* pDbgOutput = NULL;
CdbgEvents* pDbgCallbacks = NULL;
IDebugControl4* pDbgCtrl = NULL;
IDebugSymbols3* pDbgSym = NULL;
IDebugSystemObjects* pDbgSysObject = NULL;
bool g_Initialized; 

#define DEFAULT_SRC_PATH L"srv*;%TEMP%"
#define SEARCH_FLAGS (DEBUG_FIND_SOURCE_FULL_PATH | DEBUG_FIND_SOURCE_BEST_MATCH)
#define SEARCH_SERVER 0
#define SEARCH_CACHE 1

DWORD DbgiopSetupSrcPath();

BOOL DbgioInit()
{
	HRESULT hr = S_OK;
	PWSTR pBuffer = NULL;
	DWORD dwPathSize = 0;

	if (g_Initialized)
		return false;
	pDbgOutput = new CdbgOut();
	pDbgCallbacks = new CdbgEvents();
	hr = DebugCreate(__uuidof(IDebugClient8), (void**)&pDbgClient);
	LOG_IF_FAIL(hr, "DebugCreate failed with: %x\n");
	hr = pDbgClient->ConnectSession(0, 500);
	LOG_IF_FAIL(hr, "pDbgClient->ConnectSession failed with: %x\n");
	pDbgClient->SetOutputMask(DEBUG_OUTPUT_NORMAL | DEBUG_OUTPUT_ERROR);
	hr = pDbgClient->SetOutputCallbacksWide(pDbgOutput);
	LOG_IF_FAIL(hr, "pDbgClient->SetOutputCallbacksWide failed with: %x\n");
	hr = pDbgClient->SetEventCallbacks((PDEBUG_EVENT_CALLBACKS)pDbgCallbacks);
	LOG_IF_FAIL(hr, "pDbgClient->SetEventCallbacks failed with: %x\n");
	//hr = pDbgClient->GetEventCallbacks(&pDbgCallbacks);
	LOG_IF_FAIL(hr, "pDbgClient->GetEventCallbacks failed with: %x\n");
	hr = pDbgClient->GetInputCallbacks(&pDbgInput);
	LOG_IF_FAIL(hr, "pDbgClient->GetInputCallbacks failed with: %x\n");
	hr = pDbgClient->QueryInterface(__uuidof(IDebugControl4), (void**)&pDbgCtrl);
	LOG_IF_FAIL(hr, "pDbgClient->QueryInterface failed with: %x\n");
	hr = pDbgClient->QueryInterface(__uuidof(IDebugSymbols3), (void**)&pDbgSym);
	LOG_IF_FAIL(hr, "pDbgClient->QueryInterface failed with: %x\n");

	DbgiopSetupSrcPath();

	pDbgCtrl->SetExecutionStatus(DEBUG_STATUS_GO);
	g_Initialized = true;
Exit:
	return S_OK == hr;
}

DWORD DbgioInvoke(const WCHAR* szCommand)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_CONTINUE;
	hr = pDbgCtrl->ExecuteWide(DEBUG_OUTCTL_THIS_CLIENT, szCommand, NULL);
	//LOG_IF_FAIL(hr, "Execute failed with error: %x\n");
	// TODO(will): Add some logic for determining a fatal HRESULT
//Exit:
	
	return dwResult;
}

DWORD DbgioAttachDump(const WCHAR* szDumpPath)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_CONTINUE;
	if (!g_Initialized)
		return DBGIO_UNINITALIZED;

	hr = pDbgClient->OpenDumpFileWide(szDumpPath, NULL);
	LOG_IF_FAIL(hr, "OpenDumpFile failed with error: %x\n");
	hr = pDbgCtrl->WaitForEvent(0, INFINITE);
	LOG_IF_FAIL(hr, "WaitForEvent failed with error: %x\n");
	hr = pDbgClient->QueryInterface(__uuidof(IDebugSystemObjects), (void**)&pDbgSysObject);
	LOG_IF_FAIL(hr, "pDbgClient_>QueryInterface failed with: %x\n");
	//LOG_IF_FAIL(DbgiopInitSymbols(), "DbgiopInitSymbols failed with: %x\n");
	hr = S_OK;

	// Start loading symbols that are available

Exit:
	if (!SUCCEEDED(hr))
		dwResult = DBGIO_FAIL;
	SetLastError(hr);
	return dwResult;
}

DWORD DbgiopSetupSrcPath()
{
	HRESULT hr = S_OK;
	WCHAR szExpandedPath[MAX_PATH*2];
	DWORD dwResult = DBGIO_FAIL;
	ExpandEnvironmentStringsW(DEFAULT_SRC_PATH, szExpandedPath, MAX_PATH * 2);
	hr = pDbgSym->SetSourcePathWide(szExpandedPath);
	LOG_IF_FAIL(hr, "pDbgSym->SetSourcePathWide failed with:%x\n");
	dwResult = DBGIO_OK;
Exit:
	return dwResult;
}

DWORD DbgioGetCSourceFile(WCHAR** ppFilePath, DWORD* pdwLine)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_FAIL;
	ULONG64 instructionOffset = 0;
	DWORD dwLine = 0;
	WCHAR szTempPath[MAX_PATH];
	DWORD dwBufferSize = 0;
	DEBUG_STACK_FRAME stackFrame;
	CONTEXT context; // DANGER there be dragons here
	DWORD dwContextSize = sizeof(context);

	if (NULL == ppFilePath)
		goto Exit;

	// Steps
	// Find out where we are
	hr = pDbgSym->GetScope(&instructionOffset, &stackFrame, (PVOID)&context, dwContextSize);
	LOG_IF_FAIL(hr, "pDbgSym->GetScope failed with: %x\n");
	// Get file name and line offset
	hr = pDbgSym->GetLineByOffsetWide(instructionOffset, &dwLine, szTempPath, MAX_PATH * sizeof(WCHAR), NULL, NULL);
	LOG_IF_FAIL(hr, "pDbgSym->GetLineByOffsetWide failed with: %x\n");
	dwBufferSize = wcslen(szTempPath) * sizeof(WCHAR);
	*ppFilePath = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	if (NULL == *ppFilePath)
		goto Exit;
	memcpy(*ppFilePath, szTempPath, dwBufferSize);
	*pdwLine = dwLine;
	dwResult = DBGIO_OK;

Exit:
	return dwResult;
}

DWORD DbgioGetFilePath(const WCHAR* szFileName, WCHAR** szPathBuffer, DWORD dwBufferSize)
{
	HRESULT hr = S_OK;
	DWORD dwResult = DBGIO_FAIL;
	if (NULL == szPathBuffer)
		goto Exit;

	hr = pDbgSym->FindSourceFileWide(SEARCH_CACHE, szFileName, SEARCH_FLAGS, NULL, *szPathBuffer, dwBufferSize, NULL);
	if (E_NOINTERFACE == hr)
		hr = pDbgSym->FindSourceFileWide(SEARCH_SERVER, szFileName, SEARCH_FLAGS, NULL, *szPathBuffer, dwBufferSize, NULL);
	LOG_IF_FAIL(hr, "pDbgSym->FindSourceFileWide failed with: %x\n");
	dwResult = DBGIO_OK;
Exit:
	return dwResult;
}
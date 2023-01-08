// cdbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <DbgEng.h>
#include <stdio.h>
#include "debugger.h"
#include "CdbgOut.h"

#pragma comment(lib, "dbgeng.lib")
#define VERSION 1
#define FLAGS 0

CdbgOut g_dbgOutput;
HANDLE g_hRunning;
// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-debugconnectwide
// https://stackoverflow.com/questions/34470177/get-output-of-executed-windbg-command
HRESULT CdbgInit(PULONG Version, PULONG Flags);
HRESULT CdbgFunc1(PDEBUG_CLIENT Client, PCSTR Args);
int main()
{
	HANDLE hDbgThread = NULL;
	hDbgThread = CreateThread(NULL, 0, DbgMain, NULL, NULL, NULL);
	if (NULL == hDbgThread)
		goto Exit;
	WaitForSingleObject(hDbgThread, INFINITE);

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

// cdbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include "Shio.h"
#include "conwriter.h"


#define VERSION 1
#define FLAGS 0

// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-debugconnectwide
// https://stackoverflow.com/questions/34470177/get-output-of-executed-windbg-command
void ReadInput(WCHAR** ppszInput, int* pBuffSize);
BOOL DispatchLine(WCHAR* line);
BOOL ParseArgs(int argc, WCHAR* argv[]);
void ShowHelp();

int wmain(int argc, WCHAR* argv[])
{
	BOOL bRunning = TRUE;
	int buffSize = 1024;
	WCHAR* pszInput = (WCHAR*)malloc(sizeof(WCHAR) * buffSize);
	if (NULL == pszInput)
		return -1;
	ShioInit();
	while (bRunning)
	{
		ZeroMemory(pszInput, buffSize);
		CwUpdateTitle(READY);
		_cputws(L"> ");
		ReadInput(&pszInput, &buffSize);
		CwUpdateTitle(BUSY);
		bRunning = DispatchLine(pszInput);
	}
	free(pszInput);
	return 0;
}

void ReadInput(WCHAR** ppszInput, int* pBuffSize)
{
	BOOL bEOL = FALSE;
	int i = 0;
	WCHAR* szInput = *ppszInput;
	while (!bEOL)
	{
		WCHAR c = getc(stdin);
		if ('\n' == c)
			bEOL = TRUE;
		else
		{
			if (i < *pBuffSize)
				szInput[i++] = c;
		}
	}
}

BOOL DispatchLine(WCHAR* line)
{
	if (wcslen(line) == 0)
		return TRUE;
	return ShioMatchInvoke(line);
}

BOOL ParseArgs(int argc, WCHAR* argv[])
{
	if (argc == 1)
		return TRUE;
	for (int i = 1; i < argc; i++)
	{
		if (wcscmp(L"--help", argv[i]))
		{
			ShowHelp();
			return FALSE;
		}
	}
	return TRUE;
}

void ShowHelp()
{
	// TODO(will): Write me
	_putws(L"TODO(will): Write me");
}
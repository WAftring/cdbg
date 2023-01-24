// cdbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include "Shio.h"
#include "history.h"


#define VERSION 1
#define FLAGS 0

// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-debugconnectwide
// https://stackoverflow.com/questions/34470177/get-output-of-executed-windbg-command
void ReadInput(char** ppszInput, int* pBuffSize);
BOOL DispatchLine(char* line);
BOOL ParseArgs(int argc, char* argv[]);
void ShowHelp();

int main(int argc, char* argv[])
{
	BOOL bRunning = TRUE;
	int buffSize = 1024;
	char* pszInput = (char*)malloc(sizeof(char) * buffSize);
	ShioInit();
	while (bRunning)
	{
		ZeroMemory(pszInput, buffSize);
		_cputs("> ");
		ReadInput(&pszInput, &buffSize);
		bRunning = DispatchLine(pszInput);
	}
	free(pszInput);
	return 0;
}

void ReadInput(char** ppszInput, int* pBuffSize)
{
	BOOL bEOL = FALSE;
	int i = 0;
	char* szInput = *ppszInput;
	while (!bEOL)
	{
		char c = getc(stdin);
		if ('\n' == c)
			bEOL = TRUE;
		else
		{
			if (i < *pBuffSize)
				szInput[i++] = c;
		}
	}
}

BOOL DispatchLine(char* line)
{
	return ShioMatchInvoke(line);
}

BOOL ParseArgs(int argc, char* argv[])
{
	if (argc == 1)
		return TRUE;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("--help", argv[i]))
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
}
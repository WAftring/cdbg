// cdbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "Shio.h"
#include "dbgio.h"


#define VERSION 1
#define FLAGS 0

// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nf-dbgeng-debugconnectwide
// https://stackoverflow.com/questions/34470177/get-output-of-executed-windbg-command
bool DispatchLine(const char* line);
bool ParseArgs(int argc, char* argv[]);
void ShowHelp();

int main(int argc, char* argv[])
{
	bool running = true;
	std::string input;
	DbgioInit();
#if _DEBUG
	DbgioAttachDump("C:\\Users\\Will\\source\\repos\\WAftring\\cdbg\\x64\\Debug\\Notepad.exe_230114_114430.dmp");
#endif
	while (running)
	{
		std::cout << "> ";
		std::getline(std::cin, input);
		running = DispatchLine(input.c_str());
	}
	return 0;
}
bool DispatchLine(const char* line)
{
	DWORD dwResult = 0;
	if (!ShioMatchInvoke(line))
		dwResult = GetLastError();
	if (SH_NOMATCH == dwResult)
	{
		DbgioInvoke(line);
		dwResult = GetLastError();
	}
	return (dwResult == SH_CONTINUE || dwResult == DBGIO_CONTINUE);
}

bool ParseArgs(int argc, char* argv[])
{
	if (argc == 1)
		return true;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("--help", argv[i]))
		{
			ShowHelp();
			return false;
		}
		else
			return DBGIO_FAIL != DbgioAttachDump(argv[i]);
	}
}

void ShowHelp()
{
	// TODO(will): Write me
}
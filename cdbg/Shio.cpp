#include "Shio.h"
#include "dbgio.hpp"
#include "history.h"
#include "filereader.h"

#define MAX_TOKENS 1024

DWORD ShioQuit(int nArgs, LPWSTR* szArgs);	
DWORD ShioOpen(int nArgs, LPWSTR* szArgs);
DWORD ShioClose(int nArgs, LPWSTR* szArgs);
DWORD ShioDumpFile(int nArgs, LPWSTR* szArgs);
DWORD ShioClear(int nArgs, LPWSTR* szArgs);
DWORD ShioHelp(int nArgs, LPWSTR* szArgs);
DWORD ShioHistory(int nArgs, LPWSTR* szArgs);

typedef DWORD (*ShCommand)(int, LPWSTR*);
typedef struct ShFuncMatch 
{
	const WCHAR* Key;
	ShCommand Command;
} ShFuncMatch;

ShFuncMatch ShFuncMap[] =
{
	{L".quit", ShioQuit},
	{L".opendmp", ShioOpen},
	{L"df", ShioDumpFile},
	{L".close", ShioClose},
	{L".cls", ShioClear},
	{L".shelp", ShioHelp},
	{L".hist", ShioHistory}
};

void ShioInit()
{
	DbgioInit();
	HiInitHistory();
}

BOOL ShioMatchInvoke(WCHAR* line)
{
	DWORD dwResult = 0;
	BOOL bNoMatch = TRUE; 
	int nArgs = 0;
	LPWSTR *wszArgs = NULL;
	if(wcscmp(L".hist", line) != 0)
		HiAddHistory(line);
	wszArgs = CommandLineToArgvW(line, &nArgs);

	for (int i = 0; i < sizeof(ShFuncMap) / sizeof(ShFuncMap[0]); i++)
	{
		if (wcscmp(wszArgs[0], ShFuncMap[i].Key) == 0)
			return ShFuncMap[i].Command(nArgs, wszArgs) == SH_CONTINUE;
	}
	if(bNoMatch)
		return DbgioInvoke(line) == DBGIO_CONTINUE;
	return TRUE;
}

DWORD ShioQuit(int nArgs, LPWSTR* szArgs)
{
	UNREFERENCED_PARAMETER(nArgs);
	UNREFERENCED_PARAMETER(szArgs);
	return SH_STOP;
}

DWORD ShioOpen(int nArgs, LPWSTR* szArgs)
{
	// Send the path to dbgeng to try and find it
	DWORD dwResult = SH_CONTINUE;
	if(2 == nArgs)
		DbgioAttachDump(szArgs[1]);
	return dwResult;
}

DWORD ShioClose(int nArgs, LPWSTR* szArgs)
{
	// TODO(will): Implement me
	DWORD dwResult = SH_CONTINUE;
	return dwResult;
}

DWORD ShioDumpFile(int nArgs, LPWSTR* szArgs)
{
	// TODO(will): I need to figure out how to handle source...
	WCHAR *szFileName = NULL;
	WCHAR szFilePath[MAX_PATH];
	DWORD dwLine = 0;
	if (DBGIO_OK == DbgioGetCSourceFile(&szFileName, &dwLine))
	{
		if(DBGIO_OK == DbgioGetFilePath(szFileName, (WCHAR**)&szFilePath, MAX_PATH * sizeof(WCHAR)))
			FrReadSourceContext((WCHAR*)szFilePath, dwLine);
	}
	return DBGIO_OK;
}


DWORD ShioClear(int nArgs, LPWSTR* szArgs)
{
	UNREFERENCED_PARAMETER(nArgs);
	UNREFERENCED_PARAMETER(szArgs);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return SH_CONTINUE;

	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

	fill.Char.UnicodeChar = L' ';
	fill.Attributes = csbi.wAttributes;

	ScrollConsoleScreenBufferW(hConsole, &scrollRect, NULL, scrollTarget, &fill);
	
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	return SH_CONTINUE;
}

DWORD ShioHelp(int nArgs, LPWSTR* szArgs)
{
	UNREFERENCED_PARAMETER(nArgs);
	UNREFERENCED_PARAMETER(szArgs);
	_putws(L"Console Commands");
	for (int i = 0; i < sizeof(ShFuncMap) / sizeof(ShFuncMap[0]); i++)
		wprintf(L"%s\n", ShFuncMap[i].Key);
	return SH_CONTINUE;
}

DWORD ShioHistory(int nArgs, LPWSTR* szArgs)
{
	// TODO(will): Print a specific history
	HiPrintHistory();
	return SH_CONTINUE;
}
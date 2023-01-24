#include "Shio.h"
#include "dbgio.hpp"

#define MAX_TOKENS 1024

DWORD ShioQuit(const char* line);	
DWORD ShioOpen(const char* line);
DWORD ShioClose(const char* line);
DWORD ShioClear(const char* line);
DWORD ShioLoadExt(const char* line);

typedef DWORD (*ShCommand)(const char*);
typedef struct ShFuncMatch 
{
	const char* Key;
	ShCommand Command;
} ShFuncMatch;

ShFuncMatch ShFuncMap[] =
{
	{".quit", ShioQuit},
	{".dopen", ShioOpen},
	{".dclose", ShioClose},
	{".cls", ShioClear}
};

void ShioInit()
{
	DbgioInit();
}

BOOL ShioMatchInvoke(char* line)
{
	DWORD dwResult = 0;
	BOOL bNoMatch = TRUE; 


	for (int i = 0; i < sizeof(ShFuncMap) / sizeof(ShFuncMap[0]); i++)
	{
		if (strcmp(line, ShFuncMap[i].Key) == 0)
			return ShFuncMap[i].Command(line) == SH_CONTINUE;
	}
	if(bNoMatch)
		return DbgioInvoke(line) == DBGIO_CONTINUE;
	return TRUE;
}

DWORD ShioQuit(const char* line)
{
	return SH_STOP;
}

DWORD ShioOpen(const char* line)
{
	// Send the path to dbgeng to try and find it
	DWORD dwResult = SH_CONTINUE;
	DbgioAttachDump(line);
	return dwResult;
}

DWORD ShioClose(const char* line)
{
	DWORD dwResult = SH_CONTINUE;
	return dwResult;
}

DWORD ShioClear(const char* line)
{
	const char* seq = "\x1b[2J";
	DWORD dwMode, dwOld, dwError, dwWritten = 0;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleMode(hStdOut, &dwMode))
		return GetLastError();
	dwOld = dwMode;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hStdOut, dwMode))
		return GetLastError();

	WriteConsoleA(hStdOut, seq, strlen(seq), &dwWritten, NULL);
	SetConsoleMode(hStdOut, dwOld);
	return SH_CONTINUE;
}
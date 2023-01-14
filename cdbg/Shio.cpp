#include "Shio.h"
#include "dbgio.h"
#include <tuple>
#include <array>

DWORD ShioQuit(const char* line);	
DWORD ShioOpen(const char* line);
DWORD ShioClose(const char* line);
DWORD ShioClear(const char* line);
DWORD ShioLoadExt(const char* line);

typedef DWORD (*ShCommand)(const char*);
std::array<std::tuple<const char*, ShCommand>, 4> ShFuncMap =
{
	std::make_tuple(".quit", ShioQuit),
	std::make_tuple(".dopen", ShioOpen),
	std::make_tuple(".dclose", ShioClose),
	std::make_tuple(".cls", ShioClear)
};

bool ShioMatchInvoke(const char* line)
{
	DWORD dwResult = 0;
	for (auto it : ShFuncMap)
	{
		if (strcmp(std::get<0>(it), line) == 0)
		{
			dwResult = std::get<1>(it)(line);
			SetLastError(dwResult);
			return dwResult == SH_CONTINUE;
		}
	}
	SetLastError(SH_NOMATCH);
	return false;
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
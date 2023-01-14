#include "Shio.h"
#include "dbgio.h"
#include <map>

DWORD ShioQuit(const char* line);
DWORD ShioOpen(const char* line);
DWORD ShioClose(const char* line);

typedef DWORD (*ShCommand)(const char*);
std::map<const char*, ShCommand> ShFuncMap =
{
	{".quit", ShioQuit},
	{".dopen", ShioOpen},
	{".dclose", ShioClose}
};

bool ShioMatchInvoke(const char* line)
{
	if (ShFuncMap.contains(line))
	{
		SetLastError(ShFuncMap[line](line));
		return true;
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
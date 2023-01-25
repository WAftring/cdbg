#include "conwriter.h"

#define CON_READY L"CDBG - READY"
#define CON_BUSY L"CDBG - *BUSY*"

DWORD CwUpdateTitle(ConState state)
{
	if (READY == state)
		SetConsoleTitleW(CON_READY);
	else if (BUSY == state)
		SetConsoleTitleW(CON_BUSY);

	return ERROR_SUCCESS;
}
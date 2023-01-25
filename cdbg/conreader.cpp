#include "conreader.h"
#include "util.h"

void CrpProcKeyEvent(KEY_EVENT_RECORD keyEvent, ConsoleInput* pConInput);
DWORD CrGetInput(ConsoleInput* pConInput)
{
	DWORD dwError = ERROR_SUCCESS;
	INPUT_RECORD inputRecord;
	BOOL bInputDone = FALSE;
	DWORD dwNumRecords = 0;
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);

	while (!bInputDone)
	{
		WaitForSingleObject(hConsole, INFINITE);
		ReadConsoleInputW(hConsole, &inputRecord, sizeof(inputRecord), &dwNumRecords);
		if (KEY_EVENT == inputRecord.EventType)
		{
			CrpProcKeyEvent(inputRecord.Event.KeyEvent, pConInput);
		}
	}
	return dwError;
}

void CrpProcKeyEvent(KEY_EVENT_RECORD keyEvent, ConsoleInput* pConInput)
{
	// TODO(will): Think about this...
	if (keyEvent.bKeyDown)
	{

	}
}
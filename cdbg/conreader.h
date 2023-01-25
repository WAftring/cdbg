#pragma once
#include <Windows.h>

typedef struct ConsoleInput
{
	WCHAR wInput;
	WCHAR* szBuffer;

} ConsoleInput;


DWORD CrGetInput(ConsoleInput* pConInput);
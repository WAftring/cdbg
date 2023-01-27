#pragma once
#include <Windows.h>

typedef enum ConState
{
	READY,
	BUSY
};

DWORD CwUpdateTitle(ConState state);
DWORD CwAddHighlight(WCHAR* pszWord);
DWORD CwWrite(const WCHAR* pszString);
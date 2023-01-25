
#pragma once
#include <Windows.h>

typedef enum ConState
{
	READY,
	BUSY
};

// Predefined color combos


DWORD CwUpdateTitle(ConState state);
DWORD CwAddHighlight(DWORD dwColor, WCHAR* pszWord);
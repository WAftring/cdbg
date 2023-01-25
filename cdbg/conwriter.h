
#pragma once
#include <Windows.h>

typedef enum ConState
{
	READY,
	BUSY
};

DWORD CwUpdateTitle(ConState state);
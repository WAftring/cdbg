#pragma once
#include <Windows.h>

#define SH_CONTINUE 0
#define SH_STOP 1
#define SH_NOMATCH 0xFFFF

void ShioInit();
BOOL ShioMatchInvoke(WCHAR* line);
#pragma once
#include <Windows.h>
#include <string>

#define SH_CONTINUE 0
#define SH_STOP 1
#define SH_NOMATCH 0xFFFF

bool ShioMatchInvoke(const char* line);
#pragma once
#include <Windows.h>
#define MAX_HISTORY 1024

void HiInitHistory();
void HiAddHistory(const WCHAR* line);
void HiPrintHistory();
#include "history.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int g_histIndex = 0;
WCHAR* g_History[MAX_HISTORY];

void HiInitHistory()
{
	for (int i = 0; i < MAX_HISTORY; i++)
		g_History[i] = NULL;
}

void HiAddHistory(const WCHAR* line)
{
	int len = wcslen(line);
	g_History[g_histIndex] = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(WCHAR) * len);
	if (NULL == g_History[g_histIndex])
		return;
	memcpy(g_History[g_histIndex], line, len * sizeof(WCHAR));
	g_histIndex++;
	g_histIndex = g_histIndex % MAX_HISTORY;
}

void HiPrintHistory()
{
	for (int i = 0; i < MAX_HISTORY; i++)
	{
		if(NULL != g_History[i])
			_putws(g_History[i]);
	}
	_putws(L"");
}
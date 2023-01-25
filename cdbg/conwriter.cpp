#include "conwriter.h"

#define CON_READY L"CDBG - READY"
#define CON_BUSY L"CDBG - *BUSY*"

typedef struct Highlight
{
	DWORD dwColorMask;
	WCHAR* pszWord;
} Highlight;

#define MAX_HIGHLIGHT 10

DWORD g_Index;
Highlight g_Highlights[MAX_HIGHLIGHT] = 
{
	{BACKGROUND_GREEN , NULL}
}

DWORD CwUpdateTitle(ConState state)
{
	if (READY == state)
		SetConsoleTitleW(CON_READY);
	else if (BUSY == state)
		SetConsoleTitleW(CON_BUSY);

	return ERROR_SUCCESS;
}

DWORD CwAddHighlight(WCHAR* pszWord)
{
	DWORD dwBufferSize = 0;
	if (NULL == pszWord)
		return ERROR_SUCCESS;
	dwBufferSize = wcslen(pszWord) * sizeof(WCHAR);
	g_Highlights[g_Index].dwColorMask = dwColor;
	if (NULL != g_Highlights[g_Index].pszWord)
		HeapFree(GetProcessHeap(), 0, g_Highlights[g_Index].pszWord);
	g_Highlights[g_Index].pszWord = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	memcpy(g_Highlights[g_Index].pszWord, pszWord, dwBufferSize);
	g_Index = g_Index++ % MAX_HIGHLIGHT;
	return ERROR_SUCCESS;
}
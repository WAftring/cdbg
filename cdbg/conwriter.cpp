#include "conwriter.h"
#include "util.h"
#define CON_READY L"CDBG - READY"
#define CON_BUSY L"CDBG - *BUSY*"

typedef struct Highlight
{
	DWORD dwColorMask;
	WCHAR* pszWord;
} Highlight;

#define MAX_HIGHLIGHT 4

DWORD g_hlIndex = 0;
Highlight g_Highlights[MAX_HIGHLIGHT] =
{
	{BACKGROUND_INTENSITY, NULL},
	{BACKGROUND_GREEN , NULL},
	{BACKGROUND_BLUE | BACKGROUND_GREEN, NULL},
	{BACKGROUND_BLUE | BACKGROUND_RED, NULL}
};

void CwpWriteLine(const WCHAR* pszString, DWORD dwLineLength);

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
	if (NULL != g_Highlights[g_hlIndex].pszWord)
		HeapFree(GetProcessHeap(), 0, g_Highlights[g_hlIndex].pszWord);
	g_Highlights[g_hlIndex].pszWord = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	memcpy(g_Highlights[g_hlIndex].pszWord, pszWord, dwBufferSize);
	g_hlIndex++;
	g_hlIndex = g_hlIndex % MAX_HIGHLIGHT;
	return ERROR_SUCCESS;
}

DWORD CwWrite(const WCHAR* pszString)
{
	DWORD dwLineLength = 0;
	DWORD dwOffset = 0;
	BOOL bOffsetStartLB = FALSE;
	DWORD dwBufferLen = wcslen(pszString);
	for (int i = 0; i < dwBufferLen; i++)
	{
		if ('\n' == pszString[i] && i != 0)
		{
			dwLineLength = i;
			if ('\n' == pszString[dwOffset])
				dwOffset++;
			CwpWriteLine(pszString + dwOffset, (dwLineLength - dwOffset) + 1);
			dwOffset = dwLineLength;
		}
	}
	// Dealing with the final write
	//CwpWriteLine(pszString + dwOffset, dwBufferLen - dwOffset);
	return ERROR_SUCCESS;
}

void CwpWriteLine(const WCHAR* pszString, DWORD dwLineLength)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwDefaultColors = 0;
	DWORD dwBytesWritten = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	WCHAR* pszTemp = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (dwLineLength * sizeof(WCHAR)) + 2);

	memcpy(pszTemp, pszString, dwLineLength*sizeof(WCHAR));

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwDefaultColors = csbi.wAttributes;
	// Starting with simple line matches for now
	for (int i = 0; i < ARRAY_LENGTH(g_Highlights); i++)
	{
		if (NULL != g_Highlights[i].pszWord && wcsstr(pszTemp, g_Highlights[i].pszWord))
		{
			SetConsoleTextAttribute(hConsole, g_Highlights[i].dwColorMask);
			WriteConsoleW(hConsole, pszTemp, dwLineLength - 2, &dwBytesWritten, NULL);
			SetConsoleTextAttribute(hConsole, dwDefaultColors);
			WriteConsoleW(hConsole, L"\n", wcslen(L"\n"), &dwBytesWritten, NULL);
			goto Cleanup;
		}
	}

	WriteConsoleW(hConsole, pszTemp, dwLineLength, &dwBytesWritten, NULL);
	SetConsoleTextAttribute(hConsole, dwDefaultColors);
Cleanup:
	HeapFree(GetProcessHeap(), 0, pszTemp);
}
#include "filereader.h"

#define LINE_CONTEXT 10


DWORD FrReadSourceContext(WCHAR* szFilePath, DWORD dwLine)
{
	DWORD dwError = ERROR_SUCCESS;
	DWORD dwFileSize = 0;
	int iStartContext = MAX(dwLine - LINE_CONTEXT, 0);
	int iEndContext = dwLine + LINE_CONTEXT;
	int i = 0;
	int nLinesSeen = 0;
	int iStartByte = 0;
	int iEndByte = 0;
	BOOL bDone = FALSE;
	DWORD dwBytesRead = 0;
	CHAR* pBuffer = NULL;
	WCHAR* pPrintBuffer = NULL;
	HANDLE hFile = NULL;

	hFile = CreateFileW(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LOG_IF_NULL(hFile, "CreateFileW failed with error: %x\n");
	// Counting on this being a small file size...
	GetFileSize(hFile, &dwFileSize);

	pBuffer = (CHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	LOG_IF_NULL(pBuffer, "HeapAlloc failed with: %x\n");
	pPrintBuffer = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	LOG_IF_NULL(pPrintBuffer, "HeapAlloc failed with: %x\n");

	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL))
		LOG_LASTERROR("ReadFile failed with: %x\n");
	
	// Get Context lines
	while (!bDone && i < dwBytesRead)
	{
		if ('\n' == (char)pBuffer + i)
			nLinesSeen++;
		if (nLinesSeen == iStartContext)
			iStartByte = i;
		else if (nLinesSeen == iEndContext)
		{
			iEndByte = i;
			bDone = TRUE;
		}
		i++;
	}

	memcpy(pPrintBuffer, pBuffer + iStartByte, iEndByte - iStartByte);
	wprintf(pPrintBuffer);

Exit:
	return dwError;
}
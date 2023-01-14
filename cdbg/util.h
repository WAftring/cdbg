#pragma once
#include <Windows.h>
inline BOOL WIN32_FROM_HRESULT(HRESULT hr, OUT DWORD* pDWORD)
{
	if (hr < 0)
	{
		*pDWORD = (DWORD)hr;
		return TRUE;
	}
	return FALSE;
}
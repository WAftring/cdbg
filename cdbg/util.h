#pragma once
#include <Windows.h>
#include <stdio.h>

#define LOG_IF_FAIL(hr, msg) \
if (!SUCCEEDED(hr)) \
{\
	printf(msg, hr); \
	goto Exit;\
 }\


#define LOG_IF_NULL(h, msg)  \
if (NULL == h) \
{\
	dwError = GetLastError(); \
	printf(msg, dwError); \
	goto Exit;\
 }\

#define LOG_LASTERROR(msg) \
{ \
	printf(msg, GetLastError()); \
	goto Exit; \
} \

#define MIN(a, b) (((a) < (b) ? (a) : (b)))
#define MAX(a, b) (((a) > (b) ? (a) : (b)))
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))
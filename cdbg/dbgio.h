#pragma once

#include <Windows.h>
#include <stdio.h>
#include <DbgEng.h>
#include "CdbgOut.h"
#include "CdbgEvents.h"

#define DBGIO_UNINITALIZED 0
#define DBGIO_OK 1
#define DBGIO_FAIL 2
#define DBGIO_CONTINUE 3
#define DBGIO_STOP 4
#define DBGIO_NOMATCH 0xFFFF

bool DbgioInit();
bool DbgioInvoke(const char* szCommand);
DWORD DbgioAttachDump(const char* szDumpPath);
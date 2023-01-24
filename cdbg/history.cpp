#include "history.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int g_Index = 0;
char* g_History[MAX_HISTORY];

void HiInitHistory()
{
	for (int i = 0; i < MAX_HISTORY; i++)
		g_History[i] = NULL;
}

void HiAddHistory(const char* line)
{
	int len = strlen(line);
	g_History[g_Index] = (char*)malloc(sizeof(char) * len);
	memcpy(g_History[g_Index], line, len);
	g_Index++;
	g_Index = g_Index % MAX_HISTORY;
}

void HiPrintHistory()
{
	for (int i = 0; i < MAX_HISTORY; i++)
		puts(g_History[i]);
}
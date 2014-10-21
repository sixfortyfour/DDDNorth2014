// Demo - GalileoTime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <Windows.h>

void WriteToFile(HANDLE hFile, char* str);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hFile;
	char buffer[200];
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	hFile = CreateFile(L"GalileoTime.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(L"Cannot create file GalileoTime.txt");
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	_snprintf_s(buffer, sizeof(buffer), "Local time:\n");
	WriteToFile(hFile, buffer);

	_snprintf_s(buffer, sizeof(buffer), "Year:%d\nMonth:%d\nDate:%d\nHour:%d\nMin:%d\nSecond:% d\n",
											st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	WriteToFile(hFile, buffer);

	GetSystemTime(&st);
	
	_snprintf_s(buffer, sizeof(buffer), "\nSystem time:\n");

	WriteToFile(hFile, buffer);

	_snprintf_s(buffer, sizeof(buffer), "Year:%d\nMonth:%d\nDate:%d\nHour:%d\nMin:%d\nSecond:% d\n",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	WriteToFile(hFile, buffer);

	TIME_ZONE_INFORMATION tzi;

	_tprintf(L"\nOld Timezone:\n");
	GetTimeZoneInformation(&tzi);

	_tprintf(L"Standard name:%s\nDaylight name:%s\n", tzi.StandardName, tzi.DaylightName);

	// Enable the required privilege otherwise error 1314 (a required privilege is not held by the client) is returned
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_TIME_ZONE_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	TIME_ZONE_INFORMATION tzn;
	ZeroMemory(&tzn, sizeof(tzn));
	StringCchCopy(tzn.StandardName, 32, L"GMT Standard Time");
	StringCchCopy(tzn.DaylightName, 32, L"GMT Daylight Saving");

	if (!SetTimeZoneInformation(&tzn))
	{
		printf("Failed to set time zone information: (%d)\n", GetLastError());
	}

	GetTimeZoneInformation(&tzi);

	printf("\nNew Timezone:\n");
	wprintf(L"Standard name:%s\nDaylight name:%s\n", tzi.StandardName, tzi.DaylightName);

	CloseHandle(hFile);

	return 0;
}

void WriteToFile(HANDLE hFile, char* str)
{
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesWritten = 0;

	bErrorFlag = WriteFile(hFile, str, strlen(str), &dwBytesWritten, NULL);

	if (FALSE == bErrorFlag)
	{
		_tprintf(L"Failed to write file GalileoTime.txt - %d", GetLastError());
	}
}



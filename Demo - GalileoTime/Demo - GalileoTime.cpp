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

	hFile = CreateFile(L"GalileoTime.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(L"Cannot create file GalileoTime.txt");
		return 1;
	}
	else
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	_snprintf_s(buffer, sizeof(buffer), "\nLocal time:\n");
	WriteToFile(hFile, buffer);

	_snprintf_s(buffer, sizeof(buffer), "Year:%d,Month:%d,Date:%d,Hour:%d,Min:%d,Second:% d\n",
											st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	WriteToFile(hFile, buffer);

	GetSystemTime(&st);
	
	_snprintf_s(buffer, sizeof(buffer), "\nSystem time:\n");

	WriteToFile(hFile, buffer);

	_snprintf_s(buffer, sizeof(buffer), "Year:%d,Month:%d,Date:%d,Hour:%d,Min:%d,Second:% d\n",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	WriteToFile(hFile, buffer);

	TIME_ZONE_INFORMATION tzi;

	_snprintf_s(buffer, sizeof(buffer), "\nOld Timezone:\n");
	WriteToFile(hFile, buffer);

	GetTimeZoneInformation(&tzi);

	_snprintf_s(buffer, sizeof(buffer), "Standard name:%s,Daylight name:%s\n", tzi.StandardName, tzi.DaylightName);
	WriteToFile(hFile, buffer);

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

	_snprintf_s(buffer, sizeof(buffer), "\nNew Timezone:\n");
	WriteToFile(hFile, buffer);

	_snprintf_s(buffer, sizeof(buffer), "Standard name:%s,Daylight name:%s\n", tzi.StandardName, tzi.DaylightName);
	WriteToFile(hFile, buffer);

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



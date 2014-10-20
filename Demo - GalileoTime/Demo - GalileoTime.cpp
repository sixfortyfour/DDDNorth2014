// Demo - GalileoTime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	printf("Local time:\n");
	printf("Year:%d\nMonth:%d\nDate:%d\nHour:%d\nMin:%d\nSecond:% d\n",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	GetSystemTime(&st);

	printf("\nSystem time:\n");
	printf("Year:%d\nMonth:%d\nDate:%d\nHour:%d\nMin:%d\nSecond:% d\n",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	TIME_ZONE_INFORMATION tzi;

	printf("\nOld Timezone:\n");
	GetTimeZoneInformation(&tzi);

	wprintf(L"Standard name:%s\nDaylight name:%s\n", tzi.StandardName, tzi.DaylightName);

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

	return 0;
}


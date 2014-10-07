// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <winsock.h>
#include <IPHlpApi.h>

#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

int RS = 9;
int ENABLE = 8;
int D4 = 4;
int D5 = 5;
int D6 = 6;
int D7 = 7;
LiquidCrystal lcd = LiquidCrystal(RS, ENABLE, D4, D5, D6, D7); // define our LCD and which pins to use

int _tmain(int argc, _TCHAR* argv[])
{
    return RunArduinoSketch();
}

void setup()
{
	lcd.begin(16, 2); // columns and rows, LCD unit (it calls clear at the end of begin)
	lcd.setCursor(0, 0);

	DWORD len = 16;
	char cName[16];

	GetComputerNameA(cName, &len);

	lcd.print(cName);

	PIP_ADAPTER_INFO	pAdapterInfo = NULL;
	PIP_ADAPTER_INFO	pAdapter = NULL;
	ULONG				ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	DWORD				dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		lcd.setCursor(0, 1);
		lcd.print(pAdapter->IpAddressList.IpAddress.String);
	}

	if (pAdapterInfo)
		FREE(pAdapterInfo);
}

// the loop routine runs over and over again forever:
void loop()
{
    // TODO: Add your code here
}
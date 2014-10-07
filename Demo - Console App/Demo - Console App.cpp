// Demo - Console App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <winsock.h>
#include <IPHlpApi.h>

#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

int _tmain(int argc, _TCHAR* argv[])
{
	PIP_ADAPTER_INFO	pAdapterInfo = NULL;
	PIP_ADAPTER_INFO	pAdapter = NULL;
	ULONG				ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	DWORD				dwRetVal = 0;

	DWORD len = 16;
	TCHAR cName[16];

	GetComputerName(cName, &len);

	printf("\tComputer Name:\t%ws\n", cName);

	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
		printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
		printf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
		printf("\tType: \t\t");

		switch (pAdapter->Type) {
		case MIB_IF_TYPE_OTHER:
			printf("Other\n");
			break;
		case MIB_IF_TYPE_ETHERNET:
			printf("Ethernet\n");
			break;
		case MIB_IF_TYPE_TOKENRING:
			printf("Token Ring\n");
			break;
		case MIB_IF_TYPE_FDDI:
			printf("FDDI\n");
			break;
		case MIB_IF_TYPE_PPP:
			printf("PPP\n");
			break;
		case MIB_IF_TYPE_LOOPBACK:
			printf("Lookback\n");
			break;
		case MIB_IF_TYPE_SLIP:
			printf("Slip\n");
			break;
		case IF_TYPE_IEEE80211:
			printf("Wireless\n");
			break;
		default:
			printf("Unknown type %ld\n", pAdapter->Type);
			break;
		}
	}

	if (pAdapterInfo)
		FREE(pAdapterInfo);

	system("pause");

	return 0;
}


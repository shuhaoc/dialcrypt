#define WINVER 0x0501

#include <stdio.h>
#include <Windows.h>
#include <Ras.h>
#include <RasError.h>
#include "../include/dialcrypt.h"

#define ENTRY_NAME "Vnet_PPPOE"

void get_pppoe_device_name(char device_name[RAS_MaxDeviceName + 1])
{
	LPRASDEVINFO lpDeviceInfo = NULL;
	DWORD size = 0, count = 0, i;

	RasEnumDevices(NULL, &size, &count);
	lpDeviceInfo = (LPRASDEVINFO)malloc(size);
	lpDeviceInfo[0].dwSize = sizeof(RASDEVINFO);
	RasEnumDevices(lpDeviceInfo, &size, &count);
	for (i = 0; i < count; i++)
	{
		if (!strcmp(lpDeviceInfo[i].szDeviceType, "RASDT_PPPoE")	// MSDN如此云
			|| !strcmp(lpDeviceInfo[i].szDeviceType, "pppoe"))		// 但实际上是这个
		{
			strcpy(device_name, lpDeviceInfo[i].szDeviceName);
		}
	}
}

void init_ras_entry()
{
	RASENTRY entry = { 0 };
	DWORD entry_size = sizeof(entry);

	// 重新创建Vnet_PPPOE拨号连接
	entry.dwSize = sizeof(entry);
	get_pppoe_device_name(entry.szDeviceName);
	strcpy(entry.szDeviceType, "pppoe");
	entry.dwFramingProtocol = RASFP_Ppp;
	entry.dwfOptions = 0;
	entry.dwType = RASET_Broadband;
	entry.dwfNetProtocols = RASNP_Ip;
	
	RasSetEntryProperties(NULL, ENTRY_NAME, &entry, entry_size, NULL, 0);
}

int main(int argc, char* argv[]) {
	char cipher[117], errdesc[256];
	RASDIALPARAMS params = { 0 };
	HRASCONN hConn = NULL;
	DWORD error;
	BOOL bPassword;

	if (argc != 3) 
	{
		printf("usage: test username password");
		return 0;
	}
	cumt_dial_get_entire_cipher(argv[2], cipher);

	params.dwSize = sizeof(params);
	strcpy(params.szEntryName, ENTRY_NAME);
	if ((error = RasGetEntryDialParams(NULL, &params, &bPassword)) == ERROR_CANNOT_FIND_PHONEBOOK_ENTRY)
	{
		init_ras_entry();
		RasGetEntryDialParams(NULL, &params, &bPassword);
	}

	strcpy(params.szUserName, argv[1]);
	strcpy(params.szPassword, cipher);
	if ((error = RasDial(NULL, NULL, &params, 0, NULL, &hConn)) != ERROR_SUCCESS)
	{
		RasGetErrorString(error, errdesc, 256);
		printf(errdesc);
	} 
	else 
	{
		printf("连接成功！");
	}
}
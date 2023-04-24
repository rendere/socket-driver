#pragma warning(disable:28023)
#include "../Common.h"

#include "../utils/mem.h"
#include "../utils/server.h"


NTSTATUS DriverEntry(DRIVER_OBJECT pDriverObject, PUNICODE_STRING pszRegistryPath)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pszRegistryPath);

	LOG("[DEBUG] Starting driver...\r\n");
	
	UNICODE_STRING DriverName = { 0 };
	RtlInitUnicodeString(&DriverName, L"\\Driver\\TwilightSrv");
	return IoCreateDriver(
		&DriverName,
		&Init
	);
}

NTSTATUS Init(PDRIVER_OBJECT pDriver, PUNICODE_STRING pszPath)
{
	UNREFERENCED_PARAMETER(pszPath);

	// Create a system thread to handle the requests.
	if (Utils::CMemory::CreateKernelThread(&SocketThread))
	{
		LOG("[DEBUG] Failed to create the socket thread.\r\n");
		return STATUS_UNSUCCESSFUL;
	}
	
	LOG("[DEBUG] Driver loaded.\r\n");
	return STATUS_SUCCESS;
}

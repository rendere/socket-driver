#include "mem.h"


namespace Utils
{
	LPVOID CMemory::QuerySystemInformation(const SYSTEM_INFORMATION_CLASS& InfoClass)
	{
		NTSTATUS ntStatus = STATUS_SUCCESS;
		ULONG uSize = 0x1000;
		LPVOID pMemory = nullptr;

		do
		{
			pMemory = AllocateZeroedMemory(uSize);
			if (pMemory == nullptr)
			{
				LOG("[ERROR] Failed to allocate memory!\r\n");
				return nullptr;
			}

			ntStatus = ZwQuerySystemInformation(InfoClass, pMemory, uSize, &uSize);
			if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
			{
				FreeMemory(pMemory);
				pMemory = nullptr;
			}
		} while (ntStatus == STATUS_INFO_LENGTH_MISMATCH);

		if (!NT_SUCCESS(ntStatus))
		{
			FreeMemory(pMemory);
			return nullptr;
		}

		return pMemory;
	}

	NTSTATUS CMemory::GetProcessByName(const char* szProcessName, HANDLE* pProcessID)
	{
        ANSI_STRING astrProcessName;
        RtlInitAnsiString(&astrProcessName, szProcessName);

        UNICODE_STRING ustrProcessName;
        NTSTATUS status = RtlAnsiStringToUnicodeString(&ustrProcessName, &astrProcessName, true);
        if (!NT_SUCCESS(status))
            return status;

        PSYSTEM_PROCESS_INFO procInfo = (PSYSTEM_PROCESS_INFO)QuerySystemInformation(system_process_information);
        if (!procInfo)
            return status;

        PSYSTEM_PROCESS_INFO currEntry = procInfo;
        while (true)
        {
            if (!RtlCompareUnicodeString(&ustrProcessName, &currEntry->image_name, true))
            {
                if (0 >= currEntry->number_of_threads)
                {
                    if (!currEntry->next_entry_offset)
                        break;

                    currEntry = (PSYSTEM_PROCESS_INFO)( (char*)currEntry + currEntry->next_entry_offset );
                    continue;
                }

                *pProcessID = currEntry->unique_process_id;
                FreeMemory(procInfo);
                return STATUS_SUCCESS;
            }

            if (!currEntry->next_entry_offset)
                break;

            currEntry = (PSYSTEM_PROCESS_INFO)( (char*)currEntry + currEntry->next_entry_offset );
        }

        FreeMemory(procInfo);
        return STATUS_NOT_FOUND;
	}

    NTSTATUS CMemory::CreateKernelThread(PKSTART_ROUTINE pThreadRoutine, PVOID pCtx)
    {
		HANDLE hThread = nullptr;
		NTSTATUS Status = PsCreateSystemThread(&hThread, 0UL, NULL, nullptr, NULL, pThreadRoutine, pCtx);
		if (NT_SUCCESS(Status))
		{
			ZwClose(hThread);
			return Status;
		}

		if (hThread != nullptr)
			ZwClose(hThread);

		return Status;
    }
}

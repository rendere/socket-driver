#pragma once
#include "../deps/ntddk.h"


namespace Utils
{
	class CMemory
	{
	public:
		/**
		* @brief Allocates memory in the kernel space.
		* @param Size: The size of the memory to allocate.
		* @return Pointer to the allocated memory.
		*/
		static LPVOID AllocateMemory(ULONG uSize)
		{
			return ExAllocatePool2(NonPagedPool, uSize, 'MEM');
		}

		/**
		* @brief Frees memory in the kernel space.
		* @param Pointer: The pointer to the memory to free.
		*/
		__forceinline static void FreeMemory(LPVOID pMemory)
		{
			ExFreePoolWithTag(pMemory, 'MEM');
		}

		/**
		* @brief Allocates memory in the kernel space and zeroes it.
		* @param Size: The size of the memory to allocate.
		* @return Pointer to the allocated memory.
		*/
		static LPVOID AllocateZeroedMemory(ULONG uSize)
		{
			return ExAllocatePoolZero(NonPagedPool, uSize, 'MEM');
		}

		/**
		* @brief Queries system information.
		* @return Pointer to the system information use reinterpret_cast< INFO_CLASS* > to cast it.
		*/
		static LPVOID QuerySystemInformation(const SYSTEM_INFORMATION_CLASS& InfoClass);
		
		/**
		* @brief Get a process by name.
		* @param ProcessName: The name of the process to get.
		* @return NTSTATUS code.
		*/
		static NTSTATUS GetProcessByName(const char* szProcessName, HANDLE* pProcessID);

		/**
		* @brief Creates a thread in the kernel space.
		* @param pThreadRoutine: The thread routine.
		* @return NTSTATUS code.
		*/
		static NTSTATUS CreateKernelThread(PKSTART_ROUTINE pThreadRoutine, PVOID pContext = nullptr);

		/**
		* @brief Exists the current thread.
		* @param Status: The status code to exit with.
		* @return NTSTATUS code.
		*/
		__forceinline static NTSTATUS ExitThread(NTSTATUS Status)
		{
			return PsTerminateSystemThread(Status);
		}

		/**
		* @brief Sleep the current thread for x milliseconds.
		* @param Seconds: The amount of time to sleep.
		*/
		__forceinline static void SleepThread(ULONG uMilliseconds)
		{
			LARGE_INTEGER liTimeout;
			liTimeout.QuadPart = -10000 * uMilliseconds;
			KeDelayExecutionThread(KernelMode, false, &liTimeout);
		}
	};
}

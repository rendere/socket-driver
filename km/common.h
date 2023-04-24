#pragma once
#pragma warning(disable:4152)
#define _NO_CRT_STDIO_INLINE
#define _AMD64_ 1
#define _KERNEL_MODE 1
#include <ntifs.h>
#include <intrin.h>
#include <ntimage.h>
#include <windef.h>
#include <intrin.h>
#include <ntstrsafe.h>
#include <ntdef.h>
#include <stdint.h>
#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <vector>
typedef unsigned long long size_t;


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pszRegistryPath);
NTSTATUS Init(PDRIVER_OBJECT pDriver, PUNICODE_STRING pszPath);

//#ifdef _DEBUG
#define LOG(str, ...) DbgPrintEx(0, 0, str, __VA_ARGS__)
//#else
//#define LOG(str, ...)
//#endif

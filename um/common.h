#pragma once
/*
* Make sure windows is not already included,
* Also make sure we are not redefining the macros.
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifndef _INC_WINDOWS
#include <Windows.h>
#endif
#endif
#include <WinSock2.h>
#include <memory.h>
#include <string>
#include <string_view>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <format>
#include <filesystem>
#include <Shlobj.h> // SHGetKnownFolderPath :(
#include <vector>
#include <map>

// @OPTIONAL: Uncomment me if you are going to use directx rendering
// #include <d3d9.h>

typedef long NTSTATUS, * PNTSTATUS;
#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

/*
* Core includes:
*	phnt			-> Windows internals functions
*	LinuxPE			-> Module, PE, etc.. information
*	XorStr			-> String XOR'ing
*/
#include "deps/phnt/phnt_windows.h"
#include "deps/phnt/phnt.h"

#include <linuxpe>

using namespace std::chrono_literals;
#pragma once
#include <cstdint>
#include <Windows.h>
#include <vector>
#include <tlhelp32.h>
#include <stdio.h>
#include "proc/stringproc.h"

uintptr_t virtualaddy;
uintptr_t cr3;


#define CODE_RW CTL_CODE(FILE_DEVICE_UNKNOWN, 0x53AB, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_BA CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7371, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_GUARDED_REGION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5153, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_DIR_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1359, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_SECURITY 0xB15F7AF


typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

typedef struct _MEMORY_OPERATION_DATA {
	uint32_t        pid;
	ULONGLONG* cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

namespace handler
{
	HANDLE driver_handle;
	INT32 process_id;
	uintptr_t virtualaddy;
	uintptr_t cr3;

	bool find_driver()
	{
		driver_handle = CreateFileW(pr(L"\\\\.\\TazzSigmaSys").decrypt(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size)
	{
		_rw arguments = { 0 };

		arguments.security = CODE_SECURITY;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, CODE_RW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size)
	{
		_rw arguments = { 0 };

		arguments.security = CODE_SECURITY;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, CODE_RW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}



	uintptr_t fetch_cr3()
	{
		uintptr_t cr3 = NULL;
		_MEMORY_OPERATION_DATA arguments = { 0 };

		arguments.pid = process_id;
		arguments.cr3 = (ULONGLONG*)&cr3;

		DeviceIoControl(driver_handle, CODE_GET_DIR_BASE, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return cr3;
	}

	uintptr_t find_image()
	{
		uintptr_t image_address = { NULL };
		_ba arguments = { NULL };

		arguments.security = CODE_SECURITY;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, CODE_BA, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	INT32 find_process(LPCTSTR process_name)
	{
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}
}

template <typename T>
T read(uint64_t address)
{
	T buffer{ };
	handler::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer)
{
	handler::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}
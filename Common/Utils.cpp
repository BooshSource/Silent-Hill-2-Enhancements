/**
* Copyright (C) 2018 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Utils.h"
#include "..\Common\Logging.h"

std::vector<HMODULE> custom_dll;		// Used for custom dll's and asi plugins

// Search memory for byte array
void *GetAddressOfData(const void *data, size_t len, DWORD step)
{
	return GetAddressOfData(data, len, step, 0);
}

// Search memory for byte array
void *GetAddressOfData(const void *data, size_t len, DWORD step, DWORD start, DWORD distance)
{
	HANDLE hProcess = GetCurrentProcess();
	if (hProcess)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		MEMORY_BASIC_INFORMATION info;
		std::string chunk;
		BYTE* p = (BYTE*)start;
		while (p < si.lpMaximumApplicationAddress && (DWORD)p < start + distance)
		{
			if (VirtualQueryEx(hProcess, p, &info, sizeof(info)) == sizeof(info))
			{
				p = (BYTE*)info.BaseAddress;
				chunk.resize(info.RegionSize);
				SIZE_T bytesRead;
				if (ReadProcessMemory(hProcess, p, &chunk[0], info.RegionSize, &bytesRead))
				{
					for (size_t i = 0; i < (bytesRead - len); i += step)
					{
						if ((DWORD)p + i > start)
						{
							if (memcmp(data, &chunk[i], len) == 0)
							{
								return (BYTE*)p + i;
							}
						}
						if ((DWORD)p > start + distance)
						{
							return nullptr;
						}
					}
				}
				p += info.RegionSize;
			}
			else
			{
				return nullptr;
			}
		}
	}
	return nullptr;
}

// Checks the value of two data segments
bool CheckMemoryAddress(void *dataAddr, void *dataBytes, DWORD dataSize)
{
	if (!dataAddr || !dataBytes || !dataSize)
	{
		Log() << "Error: invalid memory data";
		return false;
	}

	// VirtualProtect first to make sure patch_address is readable
	DWORD dwPrevProtect;
	if (!VirtualProtect(dataAddr, dataSize, PAGE_READONLY, &dwPrevProtect))
	{
		Log() << "Error: could not read memory address";
		return false;
	}

	bool flag = (memcmp(dataAddr, dataBytes, dataSize) == 0);

	// Restore protection
	VirtualProtect(dataAddr, dataSize, dwPrevProtect, &dwPrevProtect);

	// Return results
	return flag;
}

// Update memory
bool UpdateMemoryAddress(void *dataAddr, void *dataBytes, DWORD dataSize)
{
	if (!dataAddr || !dataBytes || !dataSize)
	{
		Log() << "Error: invalid memory data";
		return false;
	}

	// VirtualProtect first to make sure patch_address is readable
	DWORD dwPrevProtect;
	if (!VirtualProtect(dataAddr, dataSize, PAGE_WRITECOPY, &dwPrevProtect))
	{
		Log() << "Error: could not write to memory address";
		return false;
	}

	// Update memory
	memcpy(dataAddr, dataBytes, dataSize);

	// Restore protection
	VirtualProtect(dataAddr, dataSize, dwPrevProtect, &dwPrevProtect);

	// Flush cache
	FlushInstructionCache(GetCurrentProcess(), dataAddr, dataSize);

	// Return
	return true;
}

// Write a jmp to memory
bool WriteJMPtoMemory(BYTE *dataAddr, void *JMPAddr)
{
	if (!dataAddr || !JMPAddr)
	{
		Log() << "Error: invalid memory data";
		return false;
	}

	// VirtualProtect first to make sure patch_address is readable
	DWORD dwPrevProtect;
	if (!VirtualProtect(dataAddr, 5, PAGE_EXECUTE_WRITECOPY, &dwPrevProtect))
	{
		Log() << "Error: Updating JMP address for PS2 Noise Filter";
		return false; // access denied
	}

	// jmp (4-byte relative)
	*dataAddr = 0xE9;
	// relative jmp address
	*((DWORD *)(dataAddr + 1)) = (DWORD)JMPAddr - (DWORD)dataAddr - 5;

	// Restore protection
	VirtualProtect(dataAddr, 5, dwPrevProtect, &dwPrevProtect);

	// Flush cache
	FlushInstructionCache(GetCurrentProcess(), dataAddr, 5);

	// Return
	return true;
}

// Add HMODULE to vector
void AddHandleToVector(HMODULE dll)
{
	if (dll)
	{
		custom_dll.push_back(dll);
	}
}

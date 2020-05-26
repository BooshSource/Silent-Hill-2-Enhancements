/**
* Copyright (C) 2020 Elisha Riedlinger
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
#include "Common\Utils.h"
#include "Logging\Logging.h"

// Variables for ASM
DWORD *CemeteryPointer;
void *jmpCemeteryAddr;

// ASM functions to update Cemetery Lighting dynamically
__declspec(naked) void __stdcall CemeteryLightingASM()
{
	__asm
	{
		pushf
		cmp ecx, 0x0001000E
		jne near CemeteryExit
		mov ecx, 0x0001000D

	CemeteryExit:
		push eax
		mov eax, dword ptr ds : [CemeteryPointer]
		mov dword ptr ds : [eax], ecx
		pop eax
		popf
		jmp jmpCemeteryAddr
	}
}

// Patch SH2 code to Fix Lighting in several rooms
void PatchRoomLighting()
{
	// Get Cemetery Lighting address
	constexpr BYTE CemeterySearchBytes[]{ 0x83, 0xEC, 0x10, 0x55, 0x56, 0x57, 0x50, 0x51, 0x8D, 0x54, 0x24, 0x14, 0x6A, 0x00, 0x52 };
	DWORD CemeteryAddr = SearchAndGetAddresses(0x0047C2DB, 0x0047C57B, 0x0047C78B, CemeterySearchBytes, sizeof(CemeterySearchBytes), 0x41);

	// Checking address pointer
	if (!CemeteryAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find memory address!";
		return;
	}
	memcpy(&CemeteryPointer, (void*)(CemeteryAddr + 2), sizeof(DWORD));
	jmpCemeteryAddr = (void*)(CemeteryAddr + 6);

	// Check for valid code before updating
	if (!CheckMemoryAddress((void*)CemeteryAddr, "\x89\x0D", 2))
	{
		Logging::Log() << __FUNCTION__ << " Error: memory addresses don't match!";
		return;
	}

	// Get carpet room lighting address
	constexpr BYTE CarpetSearchBytes[]{ 0x8B, 0x54, 0x24, 0x1C, 0x6A, 0x00, 0x89, 0x0D };
	DWORD CarpetAddr = ReadSearchedAddresses(0x00576F80, 0x00577830, 0x00577150, CarpetSearchBytes, sizeof(CarpetSearchBytes), 0x30);

	// Checking address pointer
	if (!CarpetAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find memory address!";
		return;
	}
	CarpetAddr += 0x04;

	// Update SH2 code
	Logging::Log() << "Setting Room Lighting Fix...";
	float Value = -3000.0f;
	UpdateMemoryAddress((BYTE*)CarpetAddr, &Value, sizeof(float));
	Value = -1000.0f;
	UpdateMemoryAddress((BYTE*)(CarpetAddr + 0x0C), &Value, sizeof(float));
	WriteJMPtoMemory((BYTE*)CemeteryAddr, *CemeteryLightingASM, 6);
}

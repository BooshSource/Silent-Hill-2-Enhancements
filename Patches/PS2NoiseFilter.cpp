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
#include "Common\Utils.h"
#include "Common\Logging.h"

// Predefined code bytes
constexpr BYTE FilterByteEDX[2][5] = { { 0xBA, 0xFF, 0x00, 0x00, 0x00 }, { 0xBA, 0xD7, 0x01, 0x00, 0x00 } };
constexpr BYTE FilterByteMOV[2][1] = { { 0xFF },{ 0x22 } };
constexpr BYTE FilterByteJMP[] = { 0xA2, 0xC5 };

// Variables for ASM
DWORD *FilterPointer;
void *jmpFilterAddr;
constexpr float BrightnessControl = 7.4f;

// ASM function to update PS2NoiseFilter dynamically
#pragma warning(suppress: 4725)
__declspec(naked) void __stdcall NoiseFilterASM()
{
	static BYTE tmpAddr;
	static DWORD tmpVar;
	__asm
	{
		pushf
		mov tmpAddr, al
		fild dword ptr tmpAddr
		fdiv dword ptr BrightnessControl
		fistp dword ptr tmpVar
		mov eax, tmpVar
		push ecx
		mov ecx, dword ptr ds : [FilterPointer]
		mov byte ptr ds : [ecx], al
		pop ecx
		popf
		jmp jmpFilterAddr
	}
}

// Update SH2 code for PS2 Style Noise Filter
void UpdatePS2NoiseFilter()
{
	// Get PS2 filter memory address
	DWORD FilterAddrEDX = (DWORD)CheckMultiMemoryAddress((void*)0x00477E9D, (void*)0x0047813D, (void*)0x0047834D, (void*)FilterByteEDX[0], sizeof(FilterByteEDX[0]));

	// Search for address
	if (!FilterAddrEDX)
	{
		Log() << __FUNCTION__ << " searching for memory address!";
		FilterAddrEDX = (DWORD)GetAddressOfData(FilterByteEDX[0], sizeof(FilterByteEDX[0]), 1, 0x0477C1D, 1800);
	}

	// Checking address pointer
	if (!FilterAddrEDX)
	{
		Log() << __FUNCTION__ << " Error: failed to find memory address!";
		return;
	}

	// Get relative addresses
	DWORD FilterAddrMOV = FilterAddrEDX + 0x4862;
	DWORD FilterAddrJMP = FilterAddrEDX + 0x483D;
	memcpy(&FilterPointer, (void*)(FilterAddrJMP + 1), sizeof(DWORD));
	jmpFilterAddr = (void*)(FilterAddrJMP + 5);

	// Check for valid code before updating
	if (!CheckMemoryAddress((void*)FilterAddrEDX, (void*)FilterByteEDX[0], sizeof(FilterByteEDX[0])) ||
		!CheckMemoryAddress((void*)FilterAddrMOV, (void*)FilterByteMOV[0], sizeof(FilterByteMOV[0])) ||
		!CheckMemoryAddress((void*)FilterAddrJMP, (void*)FilterByteJMP, sizeof(FilterByteJMP)))
	{
		Log() << __FUNCTION__ << " Error: memory addresses don't match!";
		return;
	}

	// Update SH2 code
	Log() << "Setting PS2 Style Noise Filter...";
	UpdateMemoryAddress((void*)FilterAddrEDX, (void*)FilterByteEDX[1], sizeof(FilterByteEDX[1]));
	UpdateMemoryAddress((void*)FilterAddrMOV, (void*)FilterByteMOV[1], sizeof(FilterByteMOV[1]));
	WriteJMPtoMemory((BYTE*)FilterAddrJMP, *NoiseFilterASM);
}

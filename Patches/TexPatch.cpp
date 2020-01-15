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
#include <Shlwapi.h>
#include <fstream>
#include <string>
#include "Patches.h"
#include "Common\FileSystemHooks.h"
#include "Common\Utils.h"
#include "Common\Settings.h"
#include "Logging\Logging.h"

struct TexAddrStruct
{
	char *name;
	BYTE vDC[5];
	BYTE v10[5];
	BYTE v11[5];
};

constexpr TexAddrStruct TexAddrList[] = {
	"\\pic\\etc\\start00.tex",  { 0xB8, 0x40, 0xEC, 0xDB, 0x01 }, { 0xB8, 0x40, 0xC0, 0xDB, 0x01 }, { 0xB8, 0x40, 0xFC, 0xDB, 0x01 }
};

// Get number of textures in array
constexpr DWORD TexNum = (sizeof(TexAddrList) / sizeof(*TexAddrList));

void TextureScaling();

void UpdateTexAddr()
{
	// Get file path
	char modPath[MAX_PATH];
	char dataPath[MAX_PATH];
	GetModuleFileNameA(nullptr, dataPath, MAX_PATH);
	strcpy_s(modPath, MAX_PATH, dataPath);
	char* p_modName = strrchr(modPath, '\\');
	char* p_dataName = strrchr(dataPath, '\\');
	strcpy_s(p_modName, MAX_PATH - strlen(modPath), std::string("\\" + std::string(ModPathA) + "\\").c_str());
	strcpy_s(p_dataName, MAX_PATH - strlen(dataPath), "\\data\\");
	p_modName = strrchr(modPath, '\\');
	p_dataName = strrchr(dataPath, '\\');

	// Loop through each texture
	for (size_t x = 0; x < TexNum; x++)
	{
		do {
			// Get texture path
			strcpy_s(p_modName, MAX_PATH - strlen(modPath), TexAddrList[x].name);
			strcpy_s(p_dataName, MAX_PATH - strlen(dataPath), TexAddrList[x].name);

			// Get file size
			char* p_pPath = (PathFileExistsA(modPath)) ? modPath : dataPath;
			Logging::Log() << "Opening " << p_pPath;
			std::ifstream infile;
			infile.open(p_pPath, std::ios::binary | std::ios::in | std::ios::ate);
			DWORD size = (DWORD)infile.tellg() + 1024;
			infile.close();

			// For now just ensure that the size is at least 16 MBs
			if (size < 16777296)
			{
				size = 16777296;
			}

			void *TexAddr;

			// Find address for texture file pointer
			const DWORD start = 0x00401000;
			const DWORD distance = 0x00227FFF;
			TexAddr = GetAddressOfData(TexAddrList[x].vDC, 5, 1, start, distance);																				// Directors Cut
			TexAddr = ((DWORD)TexAddr < start || (DWORD)TexAddr > start + distance) ? GetAddressOfData(TexAddrList[x].v10, 5, 1, start, distance) : TexAddr;	// v1.0
			TexAddr = ((DWORD)TexAddr < start || (DWORD)TexAddr > start + distance) ? GetAddressOfData(TexAddrList[x].v11, 5, 1, start, distance) : TexAddr;	// v1.1
			TexAddr = ((DWORD)TexAddr < start || (DWORD)TexAddr > start + distance) ? nullptr : TexAddr;

			if (!TexAddr)
			{
				Logging::Log() << "Could not find '" << TexAddrList[x].name << "' pointer address in memory!";
				break;
			}

			// Get relative address
			TexAddr = (void*)((DWORD)TexAddr + 0x01);

			// Log message
			Logging::Log() << "Found '" << TexAddrList[x].name << "' pointer at address: " << TexAddr << " Updating...";

			// Alocate memory
			char *PtrBytes = new char[size * 8 + 1];

			// Write new memory address
			UpdateMemoryAddress(TexAddr, &PtrBytes, sizeof(void*));

		} while (false);
	}

	TextureScaling();
}

void TextureScaling()
{
	if (GameVersion != SH2V_10)
	{
		Logging::Log() << __FUNCTION__ << " Disabling 'TextureScaling' Silent Hill 2 binary v1.0 not detected!";
		return;
	}

	RUNONCE();

	// Get addresses
	float *XScaleAddress = (float*)0x004969C8;
	float *YScaleAddress = (float*)0x004969D2;
	WORD *WidthAddress = (WORD*)0x004969A3;
	WORD *XPosAddress = (WORD*)0x00496991;

	// Compute new scale
	float XScale = (float)(TextureXRes * 16);
	float YScale = (float)(TextureYRes * 16);
	Logging::Log() << __FUNCTION__ << " Setting XYScale: " << XScale << "x" << YScale;
	UpdateMemoryAddress(XScaleAddress, &XScale, sizeof(float));
	UpdateMemoryAddress(YScaleAddress, &YScale, sizeof(float));

	// Aspect ratio
	float AspectRatio = (float)TextureYRes / (float)TextureXRes;

	// Set new width and XPos
	WORD Width = (WORD)(4096 / AspectRatio);
	WORD XPos = (WORD)(61440 - (Width - 4096));
	Logging::Log() << __FUNCTION__ << " Setting Width: " << Width << " XPos: " << XPos;
	UpdateMemoryAddress(WidthAddress, &Width, sizeof(WORD));
	UpdateMemoryAddress(XPosAddress, &XPos, sizeof(WORD));
}

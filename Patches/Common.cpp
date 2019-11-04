/**
* Copyright (C) 2019 Elisha Riedlinger
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
#include "Common\Settings.h"
#include "Logging\Logging.h"

// Variables
BYTE *ChapterIDAddr = nullptr;
DWORD *CutsceneIDAddr = nullptr;
float *CutscenePosAddr = nullptr;
float *FlashlightBrightnessAddr = nullptr;
BYTE *FlashLightRenderAddr = nullptr;
BYTE *FlashlightSwitchAddr = nullptr;
float *JamesPosAddr = nullptr;
DWORD *RoomIDAddr = nullptr;
DWORD *SpecializedLightAddr1 = nullptr;
DWORD *SpecializedLightAddr2 = nullptr;

DWORD *GetRoomIDPointer()
{
	if (RoomIDAddr)
	{
		return RoomIDAddr;
	}

	// Get room ID address
	constexpr BYTE RoomIDSearchBytes[]{ 0x83, 0xF8, 0x04, 0x0F, 0x87, 0xCE, 0x00, 0x00, 0x00 };
	void *RoomFunctAddr = (void*)SearchAndGetAddresses(0x0052A4A0, 0x0052A7D0, 0x0052A0F0, RoomIDSearchBytes, sizeof(RoomIDSearchBytes), 0xD7);

	// Checking address pointer
	if (!RoomFunctAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find room ID function address!";
		return nullptr;
	}

	// Check address
	if (!CheckMemoryAddress(RoomFunctAddr, "\x83\x3D", 2))
	{
		Logging::Log() << __FUNCTION__ << " Error: memory addresses don't match!";
		return nullptr;
	}
	RoomFunctAddr = (void*)((DWORD)RoomFunctAddr + 0x02);

	memcpy(&RoomIDAddr, RoomFunctAddr, sizeof(DWORD));

	return RoomIDAddr;
}

DWORD *GetCutsceneIDPointer()
{
	if (CutsceneIDAddr)
	{
		return CutsceneIDAddr;
	}

	// Get cutscene ID address
	constexpr BYTE CutsceneIDSearchBytes[]{ 0x8B, 0x56, 0x08, 0x89, 0x10, 0x5F, 0x5E, 0x5D, 0x83, 0xC4, 0x50, 0xC3 };
	void *CutsceneFunctAddr = (void*)SearchAndGetAddresses(0x004A0293, 0x004A0543, 0x0049FE03, CutsceneIDSearchBytes, sizeof(CutsceneIDSearchBytes), 0x1D);

	// Checking address pointer
	if (!CutsceneFunctAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find cutscene ID function address!";
		return nullptr;
	}

	// Check address
	if (!CheckMemoryAddress(CutsceneFunctAddr, "\xA1", 1))
	{
		Logging::Log() << __FUNCTION__ << " Error: memory addresses don't match!";
		return nullptr;
	}
	CutsceneFunctAddr = (void*)((DWORD)CutsceneFunctAddr + 0x01);

	memcpy(&CutsceneIDAddr, CutsceneFunctAddr, sizeof(DWORD));

	return CutsceneIDAddr;
}

float *GetCutscenePosPointer()
{
	if (CutscenePosAddr)
	{
		return CutscenePosAddr;
	}

	// Get cutscene Pos address
	constexpr BYTE CutscenePosSearchBytes[]{ 0x40, 0x88, 0x54, 0x24, 0x0B, 0x88, 0x4C, 0x24, 0x0A, 0x8B, 0x4C, 0x24, 0x08, 0x8B, 0xD1, 0x89, 0x0D };
	CutscenePosAddr = (float*)ReadSearchedAddresses(0x004A04DB, 0x004A078B, 0x004A004B, CutscenePosSearchBytes, sizeof(CutscenePosSearchBytes), 0x11);

	// Checking address pointer
	if (!CutscenePosAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find cutscene Pos function address!";
		return nullptr;
	}

	return CutscenePosAddr;
}

float *GetJamesPosPointer()
{
	if (JamesPosAddr)
	{
		return JamesPosAddr;
	}

	// Get James Pos address
	constexpr BYTE JamesPosSearchBytes[]{ 0x4A, 0x8D, 0x88, 0xCC, 0x02, 0x00, 0x00, 0x89, 0x88, 0x94, 0x01, 0x00, 0x00, 0x8B, 0xC1, 0x75, 0xEF, 0x33, 0xC9, 0x89, 0x88, 0x94, 0x01, 0x00, 0x00, 0xB8 };
	void *JamesPosition = (float*)ReadSearchedAddresses(0x00538070, 0x005383A0, 0x00537CC0, JamesPosSearchBytes, sizeof(JamesPosSearchBytes), -0x10);

	// Checking address pointer
	if (!JamesPosition)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find James Pos function address!";
		return nullptr;
	}
	JamesPosAddr = (float*)((DWORD)JamesPosition + 0x1C);

	return JamesPosAddr;
}

BYTE *GetFlashLightRenderPointer()
{
	if (FlashLightRenderAddr)
	{
		return FlashLightRenderAddr;
	}

	// Get address for flashlight render
	constexpr BYTE FlashLightRenderSearchBytes[]{ 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x33, 0xC0, 0x66, 0xA3 };
	FlashLightRenderAddr = (BYTE*)ReadSearchedAddresses(0x0050A1D6, 0x0050A506, 0x00509E26, FlashLightRenderSearchBytes, sizeof(FlashLightRenderSearchBytes), 0x14);

	// Checking address pointer
	if (!FlashLightRenderAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find flashlight render address!";
		return nullptr;
	}

	return FlashLightRenderAddr;
}

BYTE *GetChapterIDPointer()
{
	if (ChapterIDAddr)
	{
		return ChapterIDAddr;
	}

	// Get address for flashlight render
	constexpr BYTE ChapterIDSearchBytes[]{ 0x00, 0x83, 0xC4, 0x04, 0xC3, 0x6A, 0x04, 0xE8 };
	ChapterIDAddr = (BYTE*)ReadSearchedAddresses(0x00446A5F, 0x00446BFF, 0x00446BFF, ChapterIDSearchBytes, sizeof(ChapterIDSearchBytes), -0x0D);

	// Checking address pointer
	if (!ChapterIDAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find chapter ID address!";
		return nullptr;
	}

	return ChapterIDAddr;
}

DWORD *GetSpecializedLightPointer1()
{
	if (SpecializedLightAddr1)
	{
		return SpecializedLightAddr1;
	}

	// Get address for flashlight render
	constexpr BYTE SpecializedLightSearchBytes[]{ 0x8B, 0x44, 0x24, 0x04, 0x8B, 0x4C, 0x24, 0x08, 0xA3 };
	SpecializedLightAddr1 = (DWORD*)ReadSearchedAddresses(0x00445630, 0x004457F0, 0x004457F0, SpecializedLightSearchBytes, sizeof(SpecializedLightSearchBytes), 0x09);

	// Checking address pointer
	if (!SpecializedLightAddr1)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find specialized light address 1!";
		return nullptr;
	}

	return SpecializedLightAddr1;
}

DWORD *GetSpecializedLightPointer2()
{
	if (SpecializedLightAddr2)
	{
		return SpecializedLightAddr2;
	}

	// Get address for flashlight render
	constexpr BYTE SpecializedLightSearchBytes[]{ 0x00, 0x00, 0x00, 0x52, 0x6A, 0x22, 0x50, 0x89, 0x1D };
	SpecializedLightAddr2 = (DWORD*)ReadSearchedAddresses(0x004FFA1B, 0x004FFD4B, 0x004FF66B, SpecializedLightSearchBytes, sizeof(SpecializedLightSearchBytes), 0x09);

	// Checking address pointer
	if (!SpecializedLightAddr2)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find specialized light address 2!";
		return nullptr;
	}

	return SpecializedLightAddr2;
}

BYTE *GetFlashlightSwitchPointer()
{
	if (FlashlightSwitchAddr)
	{
		return FlashlightSwitchAddr;
	}

	// Get address for flashlight on/off switch address
	constexpr BYTE FlashlightSwitchSearchBytes[]{ 0x83, 0xF8, 0x33, 0x53, 0x56, 0x0F, 0x87 };
	FlashlightSwitchAddr = (BYTE*)ReadSearchedAddresses(0x0043ED25, 0x0043EEE5, 0x0043EEE5, FlashlightSwitchSearchBytes, sizeof(FlashlightSwitchSearchBytes), 0x29);

	// Checking address pointer
	if (!FlashlightSwitchAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find flashlight on/off switch address!";
		return nullptr;
	}

	return FlashlightSwitchAddr;
}

float *GetFlashlightBrightnessPointer()
{
	if (FlashlightBrightnessAddr)
	{
		return FlashlightBrightnessAddr;
	}

	// Get address for flashlight brightness address
	constexpr BYTE FlashlightBrightnessSearchBytes[]{ 0x8D, 0x54, 0x24, 0x2C, 0x52, 0x8D, 0x44, 0x24, 0x40, 0x50, 0x8D, 0x4C, 0x24, 0x54, 0x51, 0x68 };
	FlashlightBrightnessAddr = (float*)ReadSearchedAddresses(0x0047B4A5, 0x0047B745, 0x0047B955, FlashlightBrightnessSearchBytes, sizeof(FlashlightBrightnessSearchBytes), 0x10);

	// Checking address pointer
	if (!FlashlightBrightnessAddr)
	{
		Logging::Log() << __FUNCTION__ << " Error: failed to find flashlight brightness address!";
		return nullptr;
	}

	return FlashlightBrightnessAddr;
}

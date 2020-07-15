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

#include "dinput8wrapper.h"

BOOL CALLBACK m_IDirectInputEnumDevice::EnumDeviceCallbackA(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
	ENUMDEVICEA *lpCallbackContext = (ENUMDEVICEA*)pvRef;

	if (lpddi && !lpCallbackContext->lpCallback)
	{
		Logging::Log() << "|- Name: '" << lpddi->tszProductName << "' GUID: " << lpddi->guidInstance;
	}

	if (lpCallbackContext->lpCallback)
	{
		return lpCallbackContext->lpCallback(lpddi, lpCallbackContext->pvRef);
	}

	return DIENUM_CONTINUE;
}

BOOL CALLBACK m_IDirectInputEnumDeviceSemantics::EnumDeviceCallbackA(LPCDIDEVICEINSTANCEA lpddi, LPDIRECTINPUTDEVICE8A lpdid, DWORD dwFlags, DWORD dwRemaining, LPVOID pvRef)
{
	ENUMDEVICESEMA *lpCallbackContext = (ENUMDEVICESEMA*)pvRef;

	if (lpdid)
	{
		lpdid = ProxyAddressLookupTableDinput8.FindAddress<m_IDirectInputDevice8A>(lpdid);
	}

	return lpCallbackContext->lpCallback(lpddi, lpdid, dwFlags, dwRemaining, lpCallbackContext->pvRef);
}

BOOL CALLBACK m_IDirectInputEnumEffect::EnumEffectCallback(LPDIRECTINPUTEFFECT pdeff, LPVOID pvRef)
{
	ENUMEFFECT *lpCallbackContext = (ENUMEFFECT*)pvRef;

	if (pdeff)
	{
		pdeff = ProxyAddressLookupTableDinput8.FindAddress<m_IDirectInputEffect>(pdeff);
	}

	if (lpCallbackContext->lpCallback)
	{
		return lpCallbackContext->lpCallback(pdeff, lpCallbackContext->pvRef);
	}

	return DIENUM_CONTINUE;
}

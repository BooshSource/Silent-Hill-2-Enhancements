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

#include "d3d8wrapper.h"
#include "Wrappers\wrapper.h"

IDirect3D8 *WINAPI Direct3DCreate8Wrapper(UINT SDKVersion)
{
	static Direct3DCreate8Proc m_pDirect3DCreate8 = (Direct3DCreate8Proc)d3d8::Direct3DCreate8_var;

	LPDIRECT3D8 pD3D8 = m_pDirect3DCreate8(SDKVersion);

	if (pD3D8)
	{
		return new m_IDirect3D8(pD3D8);
	}

	return nullptr;
}

FARPROC p_Direct3DCreate8Wrapper = (FARPROC)*Direct3DCreate8Wrapper;

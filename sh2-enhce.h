#pragma once

#include "BuildNo.rc"

// Main resource file details
#define APP_NAME				"DxWrapper Stub"
#define APP_MAJOR				1
#define APP_MINOR				0
#define APP_BUILDNUMBER			BUILD_NUMBER
#define APP_REVISION			0
#define APP_COMPANYNAME			"Sadrate Presents"
#define APP_DESCRPTION			"Stub for DxWrapper. Supports following dlls: bcrypt.dll, cryptsp.dll, d2d1.dll, d3d8.dll, d3d9.dll, d3d10.dll, d3d10core.dll, d3d11.dll, d3d12.dll, d3dim.dll, d3dim700.dll, dciman32.dll, ddraw.dll, dinput.dll, dinput8.dll, dplayx.dll, dsound.dll, dxgi.dll, msacm32.dll, msvfw32.dll, vorbisfile.dll, winmm.dll, winmmbase.dll, winspool.drv and xlive.dll"
#define APP_COPYRIGHT			"Copyright (C) 2017 Elisha Riedlinger"
#define APP_ORIGINALVERSION		"stub.dll"
#define APP_INTERNALNAME		"DxWrapper Stub"

// Get APP_VERSION
#define _TO_STRING_(x) #x
#define _TO_STRING(x) _TO_STRING_(x)
#define APP_VERSION _TO_STRING(APP_MAJOR) "." _TO_STRING(APP_MINOR) "." _TO_STRING(APP_BUILDNUMBER) "." _TO_STRING(APP_REVISION)

#pragma once

// Shared Function declarations
void *GetCutsceneIDPointer();
void *GetCutscenePosPointer();
BYTE *GetFlashLightRenderPointer();
void *GetJamesPosPointer();
void *GetRoomIDPointer();

// Function forward declaration
void DisableCDCheck();
void SetWindowHandle(HWND WindowHandle);
void Update2TBHardDriveFix();
void UpdateBloodSize(DWORD *SH2_RoomID);
void UpdateCatacombsMeatRoom();
void UpdateCemeteryLighting();
void UpdateClosetCutscene(DWORD *SH2_CutsceneID, float *SH2_CutsceneCameraPos);
void UpdateCreatureVehicleSpawn();
void UpdateCustomExeStr();
void UpdateCustomFonts();
void UpdateDPadMovement();
void UpdateDrawDistance();
void UpdateDynamicDrawDistance(DWORD *SH2_RoomID);
void UpdateFogParameters();
void UpdateGameLoad(DWORD *SH2_RoomID, float *SH2_JamesPos);
void UpdateHangOnEsc();
void UpdateHospitalChase(DWORD *SH2_RoomID, float *SH2_JamesPos);
void UpdateHotelWater(DWORD *SH2_RoomID);
void UpdateInfiniteRumble(DWORD *SH2_RoomID);
void UpdateLightingTransition(DWORD *SH2_CutsceneID);
void UpdateMainMenuFix();
void UpdateMainMenuTitlePerLang();
void UpdatePistonRoom();
void UpdatePS2Flashlight();
void UpdatePS2NoiseFilter();
void UpdateRedCrossInCutscene();
void UpdateResolutionLock(DWORD ResX, DWORD ResY);
void UpdateRoom312ShadowFix();
void UpdateRowboatAnimation();
void UpdateSFXAddr();
void UnhookWindowHandle();
void UpdateXInputVibration();

// Varable forward declaration
extern void *CutsceneIDAddr;
extern void *CutscenePosAddr;
extern BYTE *FlashLightRenderAddr;
extern void *JamesPosAddr;
extern void *RoomIDAddr;

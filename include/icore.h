// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_ICORE_H
#define CLASSICSPATCH_ICORE_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Retrieve release version number
PATCH_API PatchVer_t PATCH_CALLTYPE ClassicsCore_GetVersion(void);

// Retrieve version of Classics Patch as a printable string
// Not the same as simply doing MakeVersionString( strVersion, ClassicsPatchAPI_GetVersion() )
PATCH_API const PatchVerString_t &PATCH_CALLTYPE ClassicsCore_GetVersionName(void);

// Get type of the application that Classics Patch is running on
// Always returns k_EClassicsPatchAppType_Unknown until ClassicsPatchAPI_Setup is called
PATCH_API EClassicsPatchAppType PATCH_CALLTYPE ClassicsCore_GetAppType(void);

// Wrappers for convenience
inline bool ClassicsCore_IsGameApp(void)    { return (ClassicsCore_GetAppType() == k_EClassicsPatchAppType_Game); };
inline bool ClassicsCore_IsServerApp(void)  { return (ClassicsCore_GetAppType() == k_EClassicsPatchAppType_Server); };
inline bool ClassicsCore_IsEditorApp(void)  { return (ClassicsCore_GetAppType() == k_EClassicsPatchAppType_Editor); };
inline bool ClassicsCore_IsModelerApp(void) { return (ClassicsCore_GetAppType() == k_EClassicsPatchAppType_Modeler); };

// Get current seasonal event
// Always returns k_EClassicsPatchSeason_None until ClassicsPatchAPI_Init is called
PATCH_API EClassicsPatchSeason PATCH_CALLTYPE ClassicsCore_GetSeason(void);

// Check if Classics Patch is using its own custom mod over vanilla entity logic
PATCH_API bool PATCH_CALLTYPE ClassicsCore_IsCustomModActive(void);

// Establish whether or not custom mod is going to be used over vanilla entity logic
//
// This method can be called only once and only *before* Serious Engine initialization
// to ensure logical consistency. ClassicsPatch_Init() may automatically and permanently
// set it to false, if the method is never called
PATCH_API void PATCH_CALLTYPE ClassicsCore_SetCustomMod(bool bState);

// Get library extension that's used for vanilla libraries (e.g. "MP" for The Second Encounter)
// It explicitly reads from ModEXT.txt of the game itself, even if any mod is currently running
PATCH_API const char *PATCH_CALLTYPE ClassicsCore_GetVanillaExt(void);

// Get full path to the currently loaded Entities library relative to the game directory
// For example: "Bin\\EntitiesMP.dll"
PATCH_API const char *PATCH_CALLTYPE ClassicsCore_GetEntitiesPath(void);

// Returns true if the Entities library is from the currently active mod instead of the base game
PATCH_API bool PATCH_CALLTYPE ClassicsCore_IsEntitiesModded(void);

// Get handle to the main engine library
PATCH_API HMODULE PATCH_CALLTYPE ClassicsCore_GetEngineHandle(void);

// Get handle to the currently loaded Entities library
PATCH_API HMODULE PATCH_CALLTYPE ClassicsCore_GetEntitiesHandle(void);

// Retrieve address from the main engine library by a symbol name
PATCH_API void *PATCH_CALLTYPE ClassicsCore_GetEngineSymbol(const char *strSymbol);

// Retrieve address from the currently loaded Entities library by a symbol name
PATCH_API void *PATCH_CALLTYPE ClassicsCore_GetEntitiesSymbol(const char *strSymbol);

#endif // CLASSICSPATCH_ICORE_H

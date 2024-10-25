// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_ICONFIG_H
#define CLASSICSPATCH_ICONFIG_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

enum EConfigProps
{
  // Mounting properties
  k_EConfigProps_TFEMount         = 0, // [bool]
  k_EConfigProps_TFEDir           = 1, // [string]
  k_EConfigProps_SSRMount         = 2, // [bool]
  k_EConfigProps_SSRDir           = 3, // [string]
  k_EConfigProps_SSRWorkshopMount = 4, // [bool]
  k_EConfigProps_SSRWorkshopDir   = 5, // [string]

  // Global properties
  k_EConfigProps_CustomMod          = 20, // [bool]
  k_EConfigProps_DebugPatcher       = 21, // [bool]
  k_EConfigProps_DPIAware           = 22, // [bool]
  k_EConfigProps_ExtendedFileSystem = 23, // [bool]
  k_EConfigProps_FullAppIntegration = 24, // [bool]
  k_EConfigProps_NotifyAboutUpdates = 25, // [bool]

  // Steam API
  k_EConfigProps_SteamEnable     = 40, // [bool] Initialize and use Steamworks API
  k_EConfigProps_SteamForServers = 41, // [bool] Initialize for dedicated servers
  k_EConfigProps_SteamForTools   = 42, // [bool] Initialize for tool applications

  k_EConfigProps_Max,
};

// Check whether a property value of some type evaluates to true
PATCH_API bool PATCH_CALLTYPE ClassicsConfig_IsTrue(EConfigProps eProperty);

// Retrieve value from an integer property
PATCH_API int PATCH_CALLTYPE ClassicsConfig_GetInt(EConfigProps eProperty);

// Retrieve value from a float property
PATCH_API float PATCH_CALLTYPE ClassicsConfig_GetFloat(EConfigProps eProperty);

// Retrieve value from a string property
PATCH_API const char *PATCH_CALLTYPE ClassicsConfig_GetString(EConfigProps eProperty);

enum EGameplayExt
{
  // Global
  k_EGameplayExt_Enable              = 0, // [bool] Utilize gameplay extensions (if disabled, resets other settings to vanilla)
  k_EGameplayExt_FixTimers           = 1, // [bool] Fix timers for entity logic to delay slowdown bug
  k_EGameplayExt_GravityAcc          = 2, // [float] Gravity acceleration multiplier

  // Custom mod only
  k_EGameplayExt_UnlimitedAirControl = 10, // [bool] Let players move while in air indefinitely
  k_EGameplayExt_MoveSpeed           = 11, // [float] Movement speed multiplier
  k_EGameplayExt_JumpHeight          = 12, // [float] Jump height multiplier
  k_EGameplayExt_FastKnife           = 13, // [bool] Increase movement speed while holding a knife in Cooperative mode

  k_EGameplayExt_Max,
};

// Check whether a gameplay extension evaluates to true
PATCH_API bool PATCH_CALLTYPE ClassicsGameplayExt_IsTrue(EGameplayExt eExtension);

// Retrieve value from an integer gameplay extension
PATCH_API int PATCH_CALLTYPE ClassicsGameplayExt_GetInt(EGameplayExt eExtension);

// Retrieve value from a float gameplay extension
PATCH_API float PATCH_CALLTYPE ClassicsGameplayExt_GetFloat(EGameplayExt eExtension);

// Retrieve value from a string gameplay extension
PATCH_API const char *PATCH_CALLTYPE ClassicsGameplayExt_GetString(EGameplayExt eExtension);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsConfig
{
public:
  virtual bool IsTrue(EConfigProps eProperty) { return ClassicsConfig_IsTrue(eProperty); };
  virtual int GetInt(EConfigProps eProperty) { return ClassicsConfig_GetInt(eProperty); };
  virtual float GetFloat(EConfigProps eProperty) { return ClassicsConfig_GetFloat(eProperty); };
  virtual const char *GetString(EConfigProps eProperty) { return ClassicsConfig_GetString(eProperty); };
};

class IClassicsGameplayExt
{
public:
  virtual bool IsTrue(EGameplayExt eExtension) { return ClassicsGameplayExt_IsTrue(eExtension); };
  virtual int GetInt(EGameplayExt eExtension) { return ClassicsGameplayExt_GetInt(eExtension); };
  virtual float GetFloat(EGameplayExt eExtension) { return ClassicsGameplayExt_GetFloat(eExtension); };
  virtual const char *GetString(EGameplayExt eExtension) { return ClassicsGameplayExt_GetString(eExtension); };
};

#endif // CLASSICSPATCH_ICONFIG_H

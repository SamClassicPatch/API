// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_ICONFIG_H
#define CLASSICSPATCH_ICONFIG_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Opaque handle to an INI config
// Equal to the class from the XGizmo library
typedef class CIniConfig *HIniConfig;

// Create a new INI config
PATCH_API HIniConfig PATCH_CALLTYPE ClassicsINI_Create(void);

// Destroy previously created INI config
PATCH_API void PATCH_CALLTYPE ClassicsINI_Destroy(HIniConfig hINI);

// Clear the INI config
PATCH_API void PATCH_CALLTYPE ClassicsINI_Clear(HIniConfig hINI);

// Check if INI config is empty
PATCH_API bool PATCH_CALLTYPE ClassicsINI_IsEmpty(HIniConfig hINI);

// Check if some section exists
PATCH_API bool PATCH_CALLTYPE ClassicsINI_SectionExists(HIniConfig hINI, const char *strSection);

// Check if some key exists under some section
PATCH_API bool PATCH_CALLTYPE ClassicsINI_KeyExists(HIniConfig hINI, const char *strSection, const char *strKey);

// Delete key under some section or the entire section (if strKey is NULL)
PATCH_API bool PATCH_CALLTYPE ClassicsINI_Delete(HIniConfig hINI, const char *strSection, const char *strKey = NULL);

// Set value to a key under some section
PATCH_API void PATCH_CALLTYPE ClassicsINI_SetValue(HIniConfig hINI, const char *strSection, const char *strKey, const char *strValue);

// Set boolean value under a key under some section
PATCH_API void PATCH_CALLTYPE ClassicsINI_SetBoolValue(HIniConfig hINI, const char *strSection, const char *strKey, bool bValue);

// Set integer value under a key under some section
PATCH_API void PATCH_CALLTYPE ClassicsINI_SetIntValue(HIniConfig hINI, const char *strSection, const char *strKey, int iValue);

// Set float value under a key under some section
PATCH_API void PATCH_CALLTYPE ClassicsINI_SetDoubleValue(HIniConfig hINI, const char *strSection, const char *strKey, double dValue);

// Get value under a key or return a default value, if key or section doesn't exist
// The returned string is always temporary and should always be copied into a separate buffer for storage
PATCH_API const char *PATCH_CALLTYPE ClassicsINI_GetValue(HIniConfig hINI, const char *strSection, const char *strKey, const char *strDefValue);

// Get boolean value under a key or return a default value, if key or section doesn't exist
PATCH_API bool PATCH_CALLTYPE ClassicsINI_GetBoolValue(HIniConfig hINI, const char *strSection, const char *strKey, bool bDefValue);

// Get integer value under a key or return a default value, if key or section doesn't exist
PATCH_API int PATCH_CALLTYPE ClassicsINI_GetIntValue(HIniConfig hINI, const char *strSection, const char *strKey, int iDefValue);

// Get float value under a key or return a default value, if key or section doesn't exist
PATCH_API double PATCH_CALLTYPE ClassicsINI_GetDoubleValue(HIniConfig hINI, const char *strSection, const char *strKey, double dDefValue);

// Read INI config from a string
PATCH_API void PATCH_CALLTYPE ClassicsINI_Read(HIniConfig hINI, const char *str);

// Write INI config into a string
// The returned string is always temporary and should always be copied into a separate buffer for storage
PATCH_API const char *PATCH_CALLTYPE ClassicsINI_Write(HIniConfig hINI);

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
  k_EConfigProps_ExtendedInput      = 26, // [bool]
  k_EConfigProps_ForceSeasonalEvent = 27, // [int]

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

class IClassicsINI
{
public:
  virtual HIniConfig Create(void) { return ClassicsINI_Create(); };
  virtual void Destroy(HIniConfig hINI) { ClassicsINI_Destroy(hINI); };

  virtual void Clear(HIniConfig hINI) { ClassicsINI_Clear(hINI); };
  virtual bool IsEmpty(HIniConfig hINI) { return ClassicsINI_IsEmpty(hINI); };
  virtual bool SectionExists(HIniConfig hINI, const char *strSection) { return ClassicsINI_SectionExists(hINI, strSection); };
  virtual bool KeyExists(HIniConfig hINI, const char *strSection, const char *strKey) { return ClassicsINI_KeyExists(hINI, strSection, strKey); };
  virtual bool Delete(HIniConfig hINI, const char *strSection, const char *strKey = NULL) { return ClassicsINI_Delete(hINI, strSection, strKey); };

  virtual void SetValue(HIniConfig hINI, const char *strSection, const char *strKey, const char *strValue) { ClassicsINI_SetValue(hINI, strSection, strKey, strValue); };
  virtual void SetBoolValue(HIniConfig hINI, const char *strSection, const char *strKey, bool bValue) { ClassicsINI_SetBoolValue(hINI, strSection, strKey, bValue); };
  virtual void SetIntValue(HIniConfig hINI, const char *strSection, const char *strKey, int iValue) { ClassicsINI_SetIntValue(hINI, strSection, strKey, iValue); };
  virtual void SetDoubleValue(HIniConfig hINI, const char *strSection, const char *strKey, double dValue) { ClassicsINI_SetDoubleValue(hINI, strSection, strKey, dValue); };

  virtual const char *GetValue(HIniConfig hINI, const char *strSection, const char *strKey, const char *strDefValue) { return ClassicsINI_GetValue(hINI, strSection, strKey, strDefValue); };
  virtual bool GetBoolValue(HIniConfig hINI, const char *strSection, const char *strKey, bool bDefValue) { return ClassicsINI_GetBoolValue(hINI, strSection, strKey, bDefValue); };
  virtual int GetIntValue(HIniConfig hINI, const char *strSection, const char *strKey, int iDefValue) { return ClassicsINI_GetIntValue(hINI, strSection, strKey, iDefValue); };
  virtual double GetDoubleValue(HIniConfig hINI, const char *strSection, const char *strKey, double dDefValue) { return ClassicsINI_GetDoubleValue(hINI, strSection, strKey, dDefValue); };

  virtual void Read(HIniConfig hINI, const char *str) { ClassicsINI_Read(hINI, str); };
  virtual const char *Write(HIniConfig hINI) { return ClassicsINI_Write(hINI); };
};

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

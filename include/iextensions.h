// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IEXTENSIONS_H
#define CLASSICSPATCH_IEXTENSIONS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"
#include "iplugins.h"

// Opaque handle type for referencing specific extensions
typedef class CPluginModule *HPatchPlugin;

// Define extension data for Classics Patch
// Example usage:
//    CLASSICSPATCH_DEFINE_EXTENSION("PATCH_EXT_fundomizer", k_EPluginFlagGame | k_EPluginFlagServer, MakeVersion(1, 0, 0),
//      "SAM_IS_AWESOME", "Fundomizer", "Cool plugin for randomizing various gameplay aspects. Works on dedicated servers too!");
#define CLASSICSPATCH_DEFINE_EXTENSION(extension, utility, version, author, name, description) \
  PLUGINMODULE_DEFINEMETHOD(void, GETINFO) (PluginInfo_t *pOutInfo) { \
    pOutInfo->m_ulAPI = CLASSICSPATCH_INTERFACE_VERSION; \
    pOutInfo->m_ulFlags = utility; \
    pOutInfo->SetMetadata(version, author, name, description); \
    pOutInfo->m_strExtensionIdentifier = extension; \
  };

// Extension handle symbol for easy access to the extension from within itself
// Example usage:
//    extern HPatchPlugin EXTENSIONMODULE_LOCALHANDLE;
//    ClassicsExtensions_GetInt(EXTENSIONMODULE_LOCALHANDLE, "my_int", &iValue);
#define EXTENSIONMODULE_LOCALHANDLE ClassicsPatchExtension_LocalPluginHandle

// Define a local handle to this extension
// Example usage:
//    // In global scope
//    CLASSICSPATCH_DEFINE_EXTENSION_HANDLE;
//    // In some function
//    ClassicsExtensions_SetInt(EXTENSIONMODULE_LOCALHANDLE, "my_int", 0);
#define CLASSICSPATCH_DEFINE_EXTENSION_HANDLE \
  MODULE_API HPatchPlugin EXTENSIONMODULE_LOCALHANDLE = NULL

// Get current amount of available extensions
PATCH_API int PATCH_CALLTYPE ClassicsExtensions_GetExtensionCount(void);

// Retrieve a specific extension by its index in the array (from 0 to GetExtensionCount() - 1)
// The array order is never guaranteed to be the same at all times, so different calls to this function with the same index
// may return handles to different plugins. It's recommended to use a more reliable GetExtensionByName() method instead.
PATCH_API HPatchPlugin PATCH_CALLTYPE ClassicsExtensions_GetExtensionByIndex(int iExtension);

// Retrieve a specific extension by its identifier
PATCH_API HPatchPlugin PATCH_CALLTYPE ClassicsExtensions_GetExtensionByName(const char *strExtension);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsExtensions
{
public:
  virtual int GetExtensionCount(void) { return ClassicsExtensions_GetExtensionCount(); };
  virtual HPatchPlugin GetExtensionByIndex(int iExtension) { return ClassicsExtensions_GetExtensionByIndex(iExtension); };
  virtual HPatchPlugin GetExtensionByName(const char *strExtension) { return ClassicsExtensions_GetExtensionByName(strExtension); };
};

#endif // CLASSICSPATCH_IEXTENSIONS_H

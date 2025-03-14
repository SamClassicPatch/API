// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IEXTENSIONS_H
#define CLASSICSPATCH_IEXTENSIONS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Opaque handle type for referencing specific plugins
typedef struct PluginInstance_t *HPatchPlugin;

// Get current amount of available extensions
PATCH_API int PATCH_CALLTYPE ClassicsExtensions_GetExtensionCount(void);

// Retrieve a specific extension by its index in the array (from 0 to GetExtensionCount() - 1)
// The array order is never guaranteed to be the same at all times, so different calls to this function with the same index
// may return handles to different plugins. It's recommended to use a more reliable GetExtensionByName() method instead.
PATCH_API HPatchPlugin *PATCH_CALLTYPE ClassicsExtensions_GetExtensionByIndex(int iExtension);

// Retrieve a specific extension by its identifier
PATCH_API HPatchPlugin *PATCH_CALLTYPE ClassicsExtensions_GetExtensionByName(const char *strExtension);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsExtensions
{
public:
  virtual int GetExtensionCount(void) { return ClassicsExtensions_GetExtensionCount(); };
  virtual HPatchPlugin *GetExtensionByIndex(int iExtension) { return ClassicsExtensions_GetExtensionByIndex(iExtension); };
  virtual HPatchPlugin *GetExtensionByName(const char *strExtension) { return ClassicsExtensions_GetExtensionByName(strExtension); };
};

#endif // CLASSICSPATCH_IEXTENSIONS_H

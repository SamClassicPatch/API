// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IPLUGINS_H
#define CLASSICSPATCH_IPLUGINS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"
#include "pluginevents.h"
#include "plugintypes.h"

class IClassicsPlugins
{
public:
  // Register a plugin symbol and set a pointer to its shell symbol back into the structure
  virtual void RegisterSymbol(PluginSymbol_t &ps, const char *strSymbolName, const char *strDefaultValue) = 0;

  // Register a plugin method in the shell or replace an existing one
  virtual void RegisterMethod(bool bUser, const char *strReturnType, const char *strFunctionName, const char *strArgumentTypes, void *pFunction) = 0;
};

// Get pointer to the plugin interface
// Returns NULL if Classics Patch hasn't been initialized yet
PATCH_API IClassicsPlugins *PATCH_CALLTYPE ClassicsPlugins(void);

//================================================================================================//
// Plugin module API
//
// The following macros should be utilized when defining API methods for plugins. These methods
// allow Classics Patch to retrieve information about the plugin, as well as automatically manage
// its initialization (startup) and cleanup (shutdown).
//
// All of these methods are purely optional and don't have to be defined, however Classics Patch
// requires metadata to be set in order to load the library as a plugin module depending on its
// utility flags. Otherwise it simply loads the library in memory and unloads it shortly after.
//================================================================================================//

// MODULE_API defines linkage and calling conventions for exported plugin methods
#define MODULE_API extern "C" __declspec(dllexport)

// Plugin method identifiers
#define PLUGINMODULEMETHOD_GETINFO  ClassicsPatchPlugin_GetInfo
#define PLUGINMODULEMETHOD_STARTUP  ClassicsPatchPlugin_Startup
#define PLUGINMODULEMETHOD_SHUTDOWN ClassicsPatchPlugin_Shutdown

// Define plugin data for Classics Patch
// Example usage:
//    CLASSICSPATCH_DEFINE_PLUGIN(k_EPluginFlagGame | k_EPluginFlagServer, MakeVersion(1, 0, 0),
//      "SAM_IS_AWESOME", "Fundomizer", "Cool plugin for randomizing various gameplay aspects. Works on dedicated servers too!");
#define CLASSICSPATCH_DEFINE_PLUGIN(utility, version, author, name, description) \
  MODULE_API void PLUGINMODULEMETHOD_GETINFO(PluginInfo_t *pOutInfo) { \
    pOutInfo->m_ulAPI = CLASSICSPATCH_INTERFACE_VERSION; \
    pOutInfo->m_ulFlags = utility; \
    pOutInfo->SetMetadata(version, author, name, description); \
  };

// Define extension data for Classics Patch
// Example usage:
//    CLASSICSPATCH_DEFINE_EXTENSION("PATCH_EXT_fundomizer", k_EPluginFlagGame | k_EPluginFlagServer, MakeVersion(1, 0, 0),
//      "SAM_IS_AWESOME", "Fundomizer", "Cool plugin for randomizing various gameplay aspects. Works on dedicated servers too!");
#define CLASSICSPATCH_DEFINE_EXTENSION(extension, utility, version, author, name, description) \
  MODULE_API void PLUGINMODULEMETHOD_GETINFO(PluginInfo_t *pOutInfo) { \
    pOutInfo->m_ulAPI = CLASSICSPATCH_INTERFACE_VERSION; \
    pOutInfo->m_ulFlags = utility; \
    pOutInfo->SetMetadata(version, author, name, description); \
    pOutInfo->m_strExtensionIdentifier = extension; \
  };

// Define plugin startup method
// Example usage:
//    CLASSICSPATCH_PLUGIN_STARTUP(CIniConfig &props, PluginEvents_t &events) {
//      ...startup code and optional interaction with plugin properties...
//    };
#define CLASSICSPATCH_PLUGIN_STARTUP MODULE_API void PLUGINMODULEMETHOD_STARTUP

// Define plugin shutdown method
// Example usage:
//    CLASSICSPATCH_PLUGIN_SHUTDOWN(CIniConfig &props) {
//      ...shutdown code and optional interaction with plugin properties...
//    };
#define CLASSICSPATCH_PLUGIN_SHUTDOWN MODULE_API void PLUGINMODULEMETHOD_SHUTDOWN

#endif // CLASSICSPATCH_IPLUGINS_H

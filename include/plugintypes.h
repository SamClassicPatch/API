// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_PLUGINTYPES_H
#define CLASSICSPATCH_PLUGINTYPES_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Flags for setting utility types to plugins, and to be used when loading and releasing plugins
enum EPluginFlags {
  // Isn't loaded nor released by the plugin API
  k_EPluginFlagManual = 0, // Manually managed libraries (e.g. Game, GameGUI)

  // Utility types
  k_EPluginFlagEngine = (1 << 0), // Internal functionality
  k_EPluginFlagGame   = (1 << 1), // In-game functionality
  k_EPluginFlagServer = (1 << 2), // Server functionality
  k_EPluginFlagEditor = (1 << 3), // Addons for Serious Editor
  k_EPluginFlagTools  = (1 << 4), // Addons for other tools

  k_EPluginFlagAll = (1 << 5) - 1, // Suitable for everything
};

const int k_cchMaxPluginInfoString = 256;

// Information about the plugin set by the plugin (but never instantiated by it!)
struct PluginInfo_t {
  ULONG m_ulAPI; // API version the plugin utilizes
  ULONG m_ulFlags; // Utility flags from EPluginFlags

  // Metadata
  ULONG m_ulVersion; // Plugin version using MakeVersion() method
  char m_strAuthor[k_cchMaxPluginInfoString]; // Author name
  char m_strName[k_cchMaxPluginInfoString]; // Plugin display name
  char m_strDescription[k_cchMaxPluginInfoString]; // Brief description of the plugin

  // Unique internal name for a "patch extension" that can be used to get access to this plugin from external sources
  // in order to utilize its functionality. Otherwise NULL for regular plugins that wish to do their own thing in peace.
  const char *m_strExtensionIdentifier;

  inline void SetMetadata(ULONG ulVersion, const char *strAuthor, const char *strName, const char *strDescription)
  {
    m_ulVersion = ulVersion;
    CopyZeroTerminatedString(m_strAuthor, strAuthor, k_cchMaxPluginInfoString);
    CopyZeroTerminatedString(m_strName, strName, k_cchMaxPluginInfoString);
    CopyZeroTerminatedString(m_strDescription, strDescription, k_cchMaxPluginInfoString);
  };
};

// Data for registering shell symbols from plugins
// Calling ClassicsPlugins()->RegisterSymbol() sets m_pShellSymbol to the newly created shell symbol
struct PluginSymbol_t
{
  enum ESymbolType {
    k_ESymbolIndex  = 0, // INDEX
    k_ESymbolFloat  = 1, // FLOAT
    k_ESymbolString = 2, // CTString
  };

  ULONG m_ulFlags; // Only SSF_CONSTANT, SSF_PERSISTENT and SSF_USER

  // Depending on the type, an appropriate default value should be set from the union below
  int m_eSymbolType; // ESymbolType

  union {
    int m_iDefault; // Default INDEX value
    float m_fDefault; // Default FLOAT value
    const char *m_strDefault; // Default CTString value
  };

  // Pointer to the eventually registered symbol in the shell
  class CShellSymbol *m_pShellSymbol;

  // Construct from an integer value
  PluginSymbol_t(ULONG ulFlags, int iDefValue)
  {
    m_ulFlags = ulFlags;
    m_eSymbolType = k_ESymbolIndex;
    m_iDefault = iDefValue;
  };

  // Construct from a float value
  PluginSymbol_t(ULONG ulFlags, float fDefValue)
  {
    m_ulFlags = ulFlags;
    m_eSymbolType = k_ESymbolFloat;
    m_fDefault = fDefValue;
  };

  // Construct from a string value
  PluginSymbol_t(ULONG ulFlags, const char *strDefValue)
  {
    m_ulFlags = ulFlags;
    m_eSymbolType = k_ESymbolString;
    m_strDefault = strDefValue;
  };
};

// Opaque handle type for referencing specific plugins
typedef struct PluginInstance_t *HPatchPlugin;

#endif // CLASSICSPATCH_PLUGINTYPES_H

// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_EXTENSIONTYPES_H
#define CLASSICSPATCH_EXTENSIONTYPES_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

//================================================================================================//
// Extension types
//
// This header defines helper structures that are used when passing arguments into specific signals
// of default extensions that come with Classics Patch.
//
// The comments before the structures describe which signal in which extension they belong to in
// this format: "extension_identifier" - "signal_name".
// All signals expect 'void *' arguments to be pointers to structures, e.g. 'UnknownProperty_t *'.
//
// This is not strictly a part of the API but rather a way to eliminate potential mismatch of data
// between the signals and the code that calls them.
// If you are creating your own extensions, it's advised to supply them with own header files
// similar to this one for helping others pass appropriate data into your extension signals.
//================================================================================================//

// "PATCH_EXT_wldconverters" - "HandleUnknownProperty"
struct ExtArgUnknownProp_t {
  class CEntity *pen; // Entity to retrieve the property value from
  ULONG ulType; // Property type, i.e. CEntityProperty::PropertyType
  ULONG ulID;   // Property ID, i.e. what should've been in CEntityProperty::ep_ulID
  void *pValue; // Pointer to any value type
};

// "PATCH_EXT_wldconverters" - various signals
// Generic structure with a converter and optional data to pass into a signal
struct ExtArgWorldConverter_t {
  int iID; // World converter ID returned by "CreateConverter", "GetConverterForFormat" and "GetConverterByName"
  void *pData; // Specific data depending on the signal

  ExtArgWorldConverter_t() : iID(-1), pData(NULL) {};
};

// "PATCH_EXT_wldconverters" - "ReplaceClass"
struct ExtArgEclData_t {
  class CTFileName *pfnmDLL;
  class CTString *pstrClassName;
};

// "PATCH_EXT_wldconverters" - "SetMethodDestructor" via ExtArgWorldConverter_t::pData
// Additional cleanup upon destroying a world converter instance
typedef void (*FWorldConverterDestructor)(void);

// "PATCH_EXT_wldconverters" - "SetMethodPrepare" via ExtArgWorldConverter_t::pData
// Prepare the world converter before loading a new world
typedef void (*FWorldConverterPrepare)(void);

// "PATCH_EXT_wldconverters" - "SetMethodReplaceClass" via ExtArgWorldConverter_t::pData
// Dynamically replace some class from some library upon creating it
// Returns true if the class (or the library it's in) has been replaced
typedef bool (*FWorldConverterReplaceClass)(ExtArgEclData_t &eclData);

// "PATCH_EXT_wldconverters" - "SetMethodHandleProperty" via ExtArgWorldConverter_t::pData
// Handle some unknown property entity property
typedef void (*FWorldConverterHandleProperty)(const ExtArgUnknownProp_t &prop);

// "PATCH_EXT_wldconverters" - "SetMethodConvertWorld" via ExtArgWorldConverter_t::pData
// Perform a conversion on a world (e.g. reinitialize specific entities)
typedef void (*FWorldConverterConvert)(class CWorld *pwo);

#endif // CLASSICSPATCH_EXTENSIONTYPES_H

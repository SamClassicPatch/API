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
  class CEntity *pen;
  ULONG ulType;
  ULONG ulID;
  void *pValue;
};

// "PATCH_EXT_wldconverters" - "ReplaceMissingClasses"
struct ExtArgEclData_t {
  class CTFileName *pfnmDLL;
  class CTString *pstrClassName;
};

#endif // CLASSICSPATCH_EXTENSIONTYPES_H

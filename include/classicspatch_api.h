// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_API_H
#define CLASSICSPATCH_API_H
#ifdef _WIN32
  #pragma once
#endif

// Common stuff
#include "classicspatch_common.h"

// All interfaces
#include "functionpatches.h"
#include "ichat.h"
#include "iconfig.h"
#include "icore.h"
#include "igame.h"
#include "imoddata.h"
#include "iplugins.h"

//================================================================================================//
// Hooking up Classics Patch API
//
// Before hooking onto Classics Patch from external modules, you first have to make sure that
// Classics Patch is usable and whichever API version you are using is supported by it.
//================================================================================================//

// Current version of Classics Patch API
// a.k.a. the version used by the external module
#define CLASSICSPATCH_INTERFACE_VERSION 001

enum EVerifyAPIResult
{
  k_EVerifyAPIResult_OK = 0,
  k_EVerifyAPIResult_NotRunning = 1, // Classics Patch hasn't been initialized yet
  k_EVerifyAPIResult_VersionMismatch = 2, // Classics Patch appears to be out of date
};

// Verify Classics Patch API that is being hooked
// Sets error message to pOutErrMsg if the result is not OK
PATCH_API EVerifyAPIResult PATCH_CALLTYPE ClassicsPatchAPI_VerifyInternal(PatchVer_t ulInterfaceVersion, ClassicsPatchErrMsg *pOutErrMsg);

inline EVerifyAPIResult ClassicsPatchAPI_Verify(ClassicsPatchErrMsg *pOutErrMsg)
{
  return ClassicsPatchAPI_VerifyInternal(CLASSICSPATCH_INTERFACE_VERSION, pOutErrMsg);
};

// Check whether Classics Patch has been initialized yet using ClassicsPatch_Init()
PATCH_API bool PATCH_CALLTYPE ClassicsPatchAPI_IsRunning(void);

//================================================================================================//
// Classics Patch setup & shutdown
//
// For initialization and shutdown the patch from projects that implement its functionality.
// These functions should not be used by external modules, unless they're never called by the
// main project that the module is trying to interact with (e.g. via code injection).
//================================================================================================//

// Should be called *before* Serious Engine initialization (SE_InitEngine) in order to perform additional setup of Classics Patch
// It can be called just once without having to setup Classics Patch before every initialization
PATCH_API void PATCH_CALLTYPE ClassicsPatch_Setup(EClassicsPatchAppType eApplicationType);

// Should be called *after* Serious Engine initialization (SE_InitEngine) in order to start up Classics Patch
PATCH_API void PATCH_CALLTYPE ClassicsPatch_Init(void);

// Should be called *before* Serious Engine shutdown (SE_EndEngine) in order to wrap up Classics Patch
PATCH_API void PATCH_CALLTYPE ClassicsPatch_Shutdown(void);

#endif // CLASSICSPATCH_API_H

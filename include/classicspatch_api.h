// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_API_H
#define CLASSICSPATCH_API_H
#ifdef _WIN32
  #pragma once
#endif

// Common stuff
#include "classicspatch_common.h"

// All interfaces
#include "extpackets.h"
#include "functionpatches.h"
#include "ichat.h"
#include "iconfig.h"
#include "icore.h"
#include "iextensions.h"
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
#define CLASSICSPATCH_INTERFACE_VERSION 004

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
// For initialization and shutdown of the patch from projects that implement its functionality.
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

//================================================================================================//
// Classics Patch engine patches
//
// For more thorough integration via specialized patches of various Serious Engine functions.
// These functions take a name of a specific "category" of engine patches that may patch a series
// of engine functions and may register shell symbols associated with them for any customization
// that has been implemented via those function patches.
// Each function has its own unique set of categories it can take and there is no guarantee that
// any of them are shared between each other. If the category of engine patches doesn't exist, no
// patching takes place and the functions return a non-zero value to indicate an error.
//
// These functions may only be called *once* per specified category and any subsequent calls may
// cause undefined behavior! Any applied engine patches can only be undone by fully shutting down
// Classics Patch!
// These functions should not be used by external modules, unless they're never called by the
// main project that the module is trying to interact with (e.g. via code injection).
//================================================================================================//

// Should be called *before* Serious Engine initialization (SE_InitEngine) BUT *after* Classics Patch setup (ClassicsPatch_Setup)
// It can be called *only once* per specified category of engine patches and can only be undone by fully shutting down Classics Patch!
// Returns zero if the patch category has been applied and a non-zero value on error
PATCH_API int PATCH_CALLTYPE ClassicsPatch_PreInitPatches(const char *strEnginePatchesCategory);

// Should be called *after* Serious Engine initialization (SE_InitEngine) AND *after* Classics Patch initialization (ClassicsPatch_Init)
// It can be called *only once* per specified category of engine patches and can only be undone by fully shutting down Classics Patch!
// Returns zero if the patch category has been applied and a non-zero value on error
PATCH_API int PATCH_CALLTYPE ClassicsPatch_PostInitPatches(const char *strEnginePatchesCategory);

//================================================================================================//
// Virtual Classics Patch API
//
// This virtual interface can be used when there is any problem with direct linking of the core
// library (classicscore.lib) or there is any sort of need for a C++ API.
// If the API library is not linked, this interface can only be accessed *after* Classics Patch
// initialization through the game shell using "ClassicsPatchAPI" symbol like this:
//    CShellSymbol *pssAPI = _pShell->GetSymbol("ClassicsPatchAPI", TRUE);
//    IClassicsPatchAPI *pAPI = (pssAPI ? pssAPI->ss_pvValue : NULL);
//
// NOTE: The old "CoreAPI" symbol is still available but is now deprecated and acts as an alias
// for the new symbol. It is left purely for compatibility purposes in case some mod checks for
// the presence of Classics Patch. It *cannot* be used to access the old-style API!
//
// CShellSymbol::ss_pvValue of the retrieved symbol (if found) is set to the exact same value
// as returned by the ClassicsPatchAPI() method.
//================================================================================================//

// Main virtual interface for accessing other interfaces
class IClassicsPatchAPI
{
public:
  virtual IClassicsChat *Chat(void) = 0;
  virtual IClassicsConfig *Config(void) = 0;
  virtual IClassicsGameplayExt *GameplayExt(void) = 0;
  virtual IClassicsCore *Core(void) = 0;
  virtual IClassicsGame *Game(void) = 0;
  virtual IClassicsModData *ModData(void) = 0;
  virtual IClassicsPlugins *Plugins(void) = 0;
  virtual IClassicsFuncPatches *FuncPatches(void) = 0;
  virtual IClassicsPackets *Packets(void) = 0;
  virtual IClassicsExtensions *Extensions(void) = 0;
  virtual IClassicsINI *INI(void) = 0;
};

// Retrieve pointer to the main virtual interface
PATCH_API IClassicsPatchAPI *PATCH_CALLTYPE ClassicsPatchAPI(void);

// Pointers to extra functions that aren't available through the main virtual interface
struct ClassicsPatchGlobalFunctions
{
  typedef EVerifyAPIResult (PATCH_CALLTYPE *FVerifyInternal)(PatchVer_t, ClassicsPatchErrMsg *);
  typedef bool (PATCH_CALLTYPE *FIsRunning)(void);
  typedef void (PATCH_CALLTYPE *FSetup)(EClassicsPatchAppType);
  typedef void (PATCH_CALLTYPE *FInit)(void);
  typedef void (PATCH_CALLTYPE *FShutdown)(void);
  typedef int (PATCH_CALLTYPE *FPreInitPatches)(const char *);
  typedef int (PATCH_CALLTYPE *FPostInitPatches)(const char *);

  FVerifyInternal _VerifyInternal;
  FIsRunning _IsRunning;
  FSetup _Setup;
  FInit _Init;
  FShutdown _Shutdown;
  FPreInitPatches _PreInitPatches;
  FPostInitPatches _PostInitPatches;

  // Simple wrapper for ClassicsPatchAPI_Verify()
  inline EVerifyAPIResult _Verify(ClassicsPatchErrMsg *pOutErrMsg) {
    return _VerifyInternal(CLASSICSPATCH_INTERFACE_VERSION, pOutErrMsg);
  };
};

// Retrieve pointers to some global functions directly from the module
// This function should be used when the library cannot be directly linked, like this:
//    ClassicsPatchGlobalFunctions functions;
//
//    if (ClassicsPatch_GetGlobalFunctionsFromModule(&functions)) {
//      // Call functions by pointers from the struct
//    }
inline bool ClassicsPatch_GetGlobalFunctionsFromModule(ClassicsPatchGlobalFunctions *pFunctions) {
  HMODULE hCore = GetModuleHandleA("ClassicsCore.dll");

  if (hCore != NULL) {
    pFunctions->_VerifyInternal  = (ClassicsPatchGlobalFunctions::FVerifyInternal)GetProcAddress(hCore, "ClassicsPatchAPI_VerifyInternal");
    pFunctions->_IsRunning       = (ClassicsPatchGlobalFunctions::FIsRunning)GetProcAddress(hCore, "ClassicsPatchAPI_IsRunning");
    pFunctions->_Setup           = (ClassicsPatchGlobalFunctions::FSetup)GetProcAddress(hCore, "ClassicsPatch_Setup");
    pFunctions->_Init            = (ClassicsPatchGlobalFunctions::FInit)GetProcAddress(hCore, "ClassicsPatch_Init");
    pFunctions->_Shutdown        = (ClassicsPatchGlobalFunctions::FShutdown)GetProcAddress(hCore, "ClassicsPatch_Shutdown");
    pFunctions->_PreInitPatches  = (ClassicsPatchGlobalFunctions::FPreInitPatches)GetProcAddress(hCore, "ClassicsPatch_PreInitPatches");
    pFunctions->_PostInitPatches = (ClassicsPatchGlobalFunctions::FPostInitPatches)GetProcAddress(hCore, "ClassicsPatch_PostInitPatches");

    return true;
  }

  return false;
};

#endif // CLASSICSPATCH_API_H

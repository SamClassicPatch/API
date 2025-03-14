// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_FUNCTIONPATCHES_H
#define CLASSICSPATCH_FUNCTIONPATCHES_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Holder of a function patch under some hashed name
struct FuncPatch_t
{
  // Get patch identifier
  virtual const char *GetName(void) const = 0;

  // Calculate hash value of this function patch
  virtual ULONG GetHash(void) const = 0;

  // Check whether the function patch is active right now
  virtual bool IsPatched(void) const = 0;

  // Turn this function patch back on, making it active again
  // Returns true if the patch has been enabled successfully
  virtual bool Enable(void) = 0;

  // Turn this function patch off but without permanently removing it
  virtual void Disable(void) = 0;
};

// Handle type for referencing specific function patches
typedef FuncPatch_t *HFuncPatch;

// Set debug state for all function patchers
// When enabled, outputs specific patcher actions in console (and as message box popups)
// upon trying to patch a series of instructions in some function for debugging purposes
PATCH_API void PATCH_CALLTYPE FuncPatch_SetDebug(bool bState);

// Set length of instruction(s) that will be replaced with a jump instruction without manually checking them
// This function should be called before CreateOpcodePatch(), after which it resets its state
//
// This function may only be used when the patcher doesn't recognize some instruction and cannot replace it
// with a jump. Since the jump instruction is 5 bytes long, the amount of bytes should be equal to the sum of
// lengths of all opcodes in its way.
//
// For example, nBytes should be set to 7 when the function that will be patched starts with either one:
// - One instruction that's 7 bytes long
// - Two instructions that are 3 and 4 bytes long respectively
// - Multiple instructions of various lengths (1-4 bytes) that add up to 7
PATCH_API void PATCH_CALLTYPE FuncPatch_ForceRewrite(int nBytes);

// Create a new function patch by replacing some opcode with a jump to a different address
// pReplaceWithJumpOpcode - pointer to some opcode, e.g. beginning of some function
// iJumpDestinationAddress - opcode address to jump to, e.g. beginning of a different function
// strIdentifier - unique identifier that also acts as a display name
//
// If this function is used inside plugin's startup method (defined using CLASSICSPATCH_PLUGIN_STARTUP),
// the created function patch is automatically added to the patch registry of that plugin, which is used
// to automatically destroy all function patches upon plugin cleanup. You should not call DestroyPatch()
// manually for each patch in CLASSICSPATCH_PLUGIN_SHUTDOWN that was created during plugin initialization.
PATCH_API HFuncPatch PATCH_CALLTYPE CreateOpcodePatch(void *pOpcodeToReplace, long iJumpDestinationAddress, const char *strIdentifier);

// Wrapper function that takes two functions instead of addresses
// funcOld - function that will have its first opcode replaced with a jump
// funcNew - function that will be executed after the jump
// Example usage:
//    static void (*pProcessFunc)(const CTFileName &) = &ProcessScript_t;
//    extern void ProcessScript_Patched(const CTFileName &);
//    CreatePatch(pProcessFunc, &ProcessScript_Patched, "ProcessScript_t(...)");
template<class FuncType1, class FuncType2> inline
HFuncPatch CreatePatch(FuncType1 &funcOld, FuncType2 funcNew, const char *strName) {
  void *pOpcodeAddress = reinterpret_cast<void *>(&funcOld);
  long iJumpAddress = *reinterpret_cast<long *>(&funcNew);

  return CreateOpcodePatch(pOpcodeAddress, iJumpAddress, strName);
};

// Destroy a created function patch to permanently restore the opcode that was replaced with a jump
PATCH_API void PATCH_CALLTYPE DestroyPatch(HFuncPatch hPatch);

// Find function patch in the registry by its identifier
PATCH_API HFuncPatch PATCH_CALLTYPE FuncPatch_FindByName(const char *strIdentifier);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsFuncPatches
{
public:
  virtual void SetDebug(bool bState) { FuncPatch_SetDebug(bState); };
  virtual void ForceRewrite(int nBytes) { FuncPatch_ForceRewrite(nBytes); };
  virtual HFuncPatch CreateOpcodePatch(void *pOpcodeToReplace, long iJumpDestinationAddress, const char *strIdentifier) { return ::CreateOpcodePatch(pOpcodeToReplace, iJumpDestinationAddress, strIdentifier); };
  virtual void DestroyPatch(HFuncPatch hPatch) { ::DestroyPatch(hPatch); };
  virtual HFuncPatch FindByName(const char *strIdentifier) { return FuncPatch_FindByName(strIdentifier); };
};

#endif // CLASSICSPATCH_FUNCTIONPATCHES_H

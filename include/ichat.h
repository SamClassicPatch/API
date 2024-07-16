// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_ICHAT_H
#define CLASSICSPATCH_ICHAT_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// NOTE for when Serious Engine is unavailable: CTString is a wrapper for an automatically allocated buffer, roughly:
//  class CTString {
//    public: char *str_String;
//  };
// It is *not recommended* to set new buffers to strResult.str_String manually!
class CTString;

// A fixed-size buffer for exchanging strings through chat command functions
const int k_cchMaxChatCommandResultStr = 1024;
typedef char ChatCommandResultStr[k_cchMaxChatCommandResultStr];

// Pointer to the chat command function
// strResult - Message that will be shown to the client after executing the command
// iClient - index of a client that sent the command (including both players and observers)
// strArguments - extra arguments sent with the command (e.g. "abc 123" if the command was "!dummy abc 123")
//
// Return value signifies whether or not the command should be displayed in chat as a regular message
// Typically, if the command cannot/shouldn't be processed for some reason (e.g. if the client index doesn't belong to the host),
// the function must return false (but can still perform necessary internal actions), otherwise it's almost always true
typedef BOOL (*FEngineChatCommand)(CTString &strResult, INDEX iClient, const CTString &strArguments);

// Function prototype compatible with C API
// These functions should be used instead of FEngineChatCommand when Serious Engine is not available *or* your module
// is built using a compiler that differs from the one Serious Engine was build with (MSVC 6.0 for 1.05 and 1.07)
typedef BOOL (*FPureChatCommand)(ChatCommandResultStr &strResult, INDEX iClient, const char *strArguments);

// Register a new chat command with Serious Engine compatible function
PATCH_API void PATCH_CALLTYPE ClassicsChat_RegisterCommand(const char *strName, FEngineChatCommand pFunction);

// Register a new chat command with pure C function
PATCH_API void PATCH_CALLTYPE ClassicsChat_RegisterCommandPure(const char *strName, FPureChatCommand pFunction);

// Unregister a chat command by its name
PATCH_API void PATCH_CALLTYPE ClassicsChat_UnregisterCommand(const char *strName);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsChat
{
public:
  virtual void RegisterCommand(const char *strName, FEngineChatCommand pFunction) { ClassicsChat_RegisterCommand(strName, pFunction); };
  virtual void RegisterCommandPure(const char *strName, FPureChatCommand pFunction) { ClassicsChat_RegisterCommandPure(strName, pFunction); };
  virtual void UnregisterCommand(const char *strName) { ClassicsChat_UnregisterCommand(strName); };
};

#endif // CLASSICSPATCH_ICHAT_H

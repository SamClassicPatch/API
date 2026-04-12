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

// Chat command access levels
enum EChatCommandAccessLevel {
  k_EChatCommandAccessLevel_Everyone = 0, // May be executed by anyone on the server
  k_EChatCommandAccessLevel_Admin    = 1, // May be executed only by the server admins and operators
  k_EChatCommandAccessLevel_Operator = 2, // May be executed only by the server operators

  k_EChatCommandAccessLevel_Max,
};

// Callback function for determining whether a chat command is currently usable by some client
typedef BOOL (*FCheckChatCommand)(INDEX iClient);

// Register a new chat command with a Serious Engine compatible function
PATCH_API void PATCH_CALLTYPE ClassicsChat_RegisterCommand(const char *strName, FEngineChatCommand pFunction);

// Register a new chat command with a pure C function
PATCH_API void PATCH_CALLTYPE ClassicsChat_RegisterCommandPure(const char *strName, FPureChatCommand pFunction);

// Unregister a chat command by its name
PATCH_API void PATCH_CALLTYPE ClassicsChat_UnregisterCommand(const char *strName);

// Set access level for some chat command and whether to hide it from regular clients when listing it using the "!help" command
// Returns true if the access has been updated
PATCH_API BOOL PATCH_CALLTYPE ClassicsChat_SetCommandAccess(const char *strName, EChatCommandAccessLevel eAccess, BOOL bHidden);

// Set a callback function for some chat command for checking whether it's currently usable and should be listed using the "!help" command
// Returns true if the callback function has been updated
PATCH_API BOOL PATCH_CALLTYPE ClassicsChat_SetCommandCheck(const char *strName, FCheckChatCommand pFunction);

// Set useful usage information about some chat command when listing it using the "!help" command
// strArgumentList - List of arguments that the command accepts surrounded by <> (for required arguments) or by [] (for optional arguments), for example: "<index> <time> [message]"
// strDescription - Short description of the command's purpose
// Returns true if the infomation has been updated
PATCH_API BOOL PATCH_CALLTYPE ClassicsChat_SetCommandInfo(const char *strName, const char *strArgumentList, const char *strDescription);

// Returns name of the currently processed command for distinguishing behavior using the same chat command callbacks
// Only valid during execution of the following command callbacks (otherwise returns NULL):
//   FEngineChatCommand, FPureChatCommand, FCheckChatCommand
PATCH_API const char *PATCH_CALLTYPE ClassicsChat_CurrentCommand(void);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsChat
{
public:
  virtual void RegisterCommand(const char *strName, FEngineChatCommand pFunction) { ClassicsChat_RegisterCommand(strName, pFunction); };
  virtual void RegisterCommandPure(const char *strName, FPureChatCommand pFunction) { ClassicsChat_RegisterCommandPure(strName, pFunction); };
  virtual void UnregisterCommand(const char *strName) { ClassicsChat_UnregisterCommand(strName); };

  virtual BOOL SetCommandAccess(const char *strName, EChatCommandAccessLevel eAccess, BOOL bHidden) { return ClassicsChat_SetCommandAccess(strName, eAccess, bHidden); };
  virtual BOOL SetCommandInfo(const char *strName, const char *strArgumentList, const char *strDescription) { return ClassicsChat_SetCommandInfo(strName, strArgumentList, strDescription); };
  virtual BOOL SetCommandCheck(const char *strName, FCheckChatCommand pFunction) { return ClassicsChat_SetCommandCheck(strName, pFunction); };

  virtual const char *CurrentCommand(void) { return ClassicsChat_CurrentCommand(); };
};

#endif // CLASSICSPATCH_ICHAT_H

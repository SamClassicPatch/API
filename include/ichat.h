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
class CTString;

// Pointer to the chat command function
// strResult - Message that will be shown to the client after executing the command
// iClient - index of a client that sent the command (including both players and observers)
// strArguments - extra arguments sent with the command (e.g. "abc 123" if the command was "!dummy abc 123")
//
// Return value signifies whether or not the command should be displayed in chat as a regular message
// Typically, if the command cannot/shouldn't be processed for some reason (e.g. if the client index doesn't belong to the host),
// the function must return false (but can still perform necessary internal actions), otherwise it's almost always true
typedef BOOL (*FChatCommand)(CTString &strResult, INDEX iClient, const CTString &strArguments);

// Register a new chat command
PATCH_API void PATCH_CALLTYPE ClassicsChat_RegisterCommand(const char *strName, FChatCommand pFunction);

// Unregister a chat command by its name
PATCH_API void PATCH_CALLTYPE ClassicsChat_UnregisterCommand(const char *strName);

#endif // CLASSICSPATCH_ICHAT_H

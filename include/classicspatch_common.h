// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_COMMON_H
#define CLASSICSPATCH_COMMON_H
#ifdef _WIN32
  #pragma once
#endif

#include <cstdio>
#include <cstring>

// PATCH_API defines the linkage and calling conventions for ClassicsCore.dll exports
#ifdef CORELIB_EXPORTS
  #define PATCH_API extern "C" __declspec(dllexport)
#else
  #define PATCH_API extern "C" __declspec(dllimport)
#endif

#define PATCH_CALLTYPE __cdecl

// Integral types
typedef signed long  int   PatchS32; // Serious Engine 1 - SLONG
typedef signed short int   PatchS16; // Serious Engine 1 - SWORD
typedef signed char	       PatchS8;  // Serious Engine 1 - SBYTE

typedef unsigned long  int PatchU32; // Serious Engine 1 - ULONG
typedef unsigned short int PatchU16; // Serious Engine 1 - UWORD
typedef unsigned char      PatchU8;  // Serious Engine 1 - UBYTE

typedef      int PatchBOOL;  // Serious Engine 1 - BOOL
typedef long int PatchINDEX; // Serious Engine 1 - INDEX

#ifndef FALSE
  #define FALSE 0
#endif
#ifndef TRUE
  #define TRUE 1
#endif

// A fixed-size buffer to receive an error message that is returned by some API calls
const int k_cchMaxClassicsPatchErrMsg = 1024;
typedef char ClassicsPatchErrMsg[k_cchMaxClassicsPatchErrMsg];

// Types of available applications running on Serious Engine
// These types may be used to distinguish currently running applications to implement application-specific logic
enum EClassicsPatchAppType
{
  k_EClassicsPatchAppType_Unknown = 0, // Unspecified application
  k_EClassicsPatchAppType_Game    = 1, // Game Executable
  k_EClassicsPatchAppType_Server  = 2, // Dedicated Server
  k_EClassicsPatchAppType_Editor  = 3, // Serious Editor
  k_EClassicsPatchAppType_Modeler = 4, // Serious Modeler or Serious SKA Studio

  k_EClassicsPatchAppType_Max,
};

// Current seasonal events
// These types may be used to perform specific actions depending on the current event
enum EClassicsPatchSeason
{
  k_EClassicsPatchSeason_None        = 0,
  k_EClassicsPatchSeason_Valentine   = 1, // Feb 10 - Feb 18
  k_EClassicsPatchSeason_Birthday    = 2, // Mar 19 - Mar 23 (Serious Sam)
  k_EClassicsPatchSeason_Halloween   = 3, // Oct 01 - Oct 31
  k_EClassicsPatchSeason_Christmas   = 4, // Dec 15 - Jan 15
  k_EClassicsPatchSeason_Anniversary = 5, // Jun 20 - Jun 24 (Classics Patch)

  k_EClassicsPatchSeason_Max,
};

// Classics Patch version number
typedef PatchU32 PatchVer_t;
typedef char PatchVerString_t[256];

// Construct version number
inline PatchVer_t MakeVersion(PatchU8 ubRelease, PatchU8 ubUpdate, PatchU8 ubPatch) {
  return (ubRelease << 16) | (ubUpdate << 8) | (ubPatch << 0);
};

// Deconstruct version number
inline void GetVersionNumbers(PatchVer_t ulVersionNumber, PatchU8 &ubRelease, PatchU8 &ubUpdate, PatchU8 &ubPatch) {
  ubRelease = (ulVersionNumber >> 16) & 0xFF;
  ubUpdate  = (ulVersionNumber >>  8) & 0xFF;
  ubPatch   = (ulVersionNumber >>  0) & 0xFF;
};

// Retrieve version number as a printable string
// Right now the longest the version string can be is "255.255.255"
inline void MakeVersionString(PatchVerString_t &strOutVersion, PatchVer_t ulVersionNumber) {
  PatchU8 ubRelease, ubUpdate, ubPatch;
  GetVersionNumbers(ulVersionNumber, ubRelease, ubUpdate, ubPatch);

  // Discard the patch integer if it's a full release, e.g. "1.10" instead of "1.10.0"
  sprintf(strOutVersion, (ubPatch != 0) ? "%u.%u.%u" : "%u.%u", ubRelease, ubUpdate, ubPatch);
};

// If the source string is longer than the buffer, then the very last character of the buffer
// is zeroed to ensure that the destination string is always null-terminated
// For example, if strlen( pchSrc ) == 32 and pchDst was defined as 'char[32]',
// it will only copy the first 31 characters from pchSrc and set pchDst[31] to 0
inline void CopyZeroTerminatedString(char *pchDst, const char *pchSrc, size_t cchDstLength)
{
  cchDstLength -= 1; // Leave space for the null-terminator

  strncpy(pchDst, pchSrc, cchDstLength);
  pchDst[cchDstLength] = '\0'; // Set null-terminator as the last character
};

#define CLASSICSPATCH_STRINGIFY_INTERNAL(x) #x
#define CLASSICSPATCH_STRINGIFY(x) CLASSICSPATCH_STRINGIFY_INTERNAL(x)

#endif // CLASSICSPATCH_COMMON_H

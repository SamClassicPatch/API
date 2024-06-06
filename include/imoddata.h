// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IMODDATA_H
#define CLASSICSPATCH_IMODDATA_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

enum EModDataProps
{
  k_EModDataProps_AdjustFOV         = 0, // Adjust FOV depending on aspect ratio
  k_EModDataProps_AdjustAR          = 1, // Set wide adjustment based on aspect ratio
  k_EModDataProps_ProperTextScaling = 2, // Adjust scaling of menu text based on screen height instead of width
  k_EModDataProps_MenuTextScale     = 3, // Additional scale multiplication of menu text

  k_EModDataProps_Max,
};

// Check whether a mod property evaluates to true
PATCH_API bool PATCH_CALLTYPE ClassicsModData_IsTrue(EModDataProps eProperty);

// Retrieve value from an integer mod property
PATCH_API int PATCH_CALLTYPE ClassicsModData_GetInt(EModDataProps eProperty);

// Retrieve value from a float mod property
PATCH_API float PATCH_CALLTYPE ClassicsModData_GetFloat(EModDataProps eProperty);

// Retrieve value from a string mod property
PATCH_API const char *PATCH_CALLTYPE ClassicsModData_GetString(EModDataProps eProperty);

const int k_cchMaxDifficultyString = 256;

// Difficulty from a mod for the in-game menu
struct ModDifficulty_t {
  int m_iLevel; // Difficulty level (for gam_iStartDifficulty)
  bool m_bFlash; // Make text blink
  char m_strName[k_cchMaxDifficultyString]; // Display name
  char m_strTip[k_cchMaxDifficultyString]; // Difficulty description
  char m_strCommand[k_cchMaxDifficultyString]; // Allow difficulty selection if the value is TRUE

  // Default constructor
  ModDifficulty_t() : m_iLevel(0), m_bFlash(false)
  {
    strcpy(m_strName, "");
    strcpy(m_strTip, "");
    strcpy(m_strCommand, "");
  };

  // Constructor with a level and untranslated strings
  ModDifficulty_t(int iSetLevel, const char *strSetName, const char *strSetTip, const char *strSetCommand = "") :
    m_iLevel(iSetLevel), m_bFlash(false)
  {
    SetName(strSetName);
    SetTip(strSetTip);
    SetCommand(strSetCommand);
  };

  inline ModDifficulty_t &operator=(const ModDifficulty_t &diffOther)
  {
    m_iLevel = diffOther.m_iLevel;
    m_bFlash = diffOther.m_bFlash;
    strcpy(m_strName, diffOther.m_strName);
    strcpy(m_strTip, diffOther.m_strTip);
    strcpy(m_strCommand, diffOther.m_strCommand);
    return *this;
  };

  inline void SetName(const char *str)    { CopyZeroTerminatedString(m_strName,    str, k_cchMaxDifficultyString); };
  inline void SetTip(const char *str)     { CopyZeroTerminatedString(m_strTip,     str, k_cchMaxDifficultyString); };
  inline void SetCommand(const char *str) { CopyZeroTerminatedString(m_strCommand, str, k_cchMaxDifficultyString); };
};

// Get a specific game difficulty from the array
// Can be used for configuring custom difficulties or retrieving data about existing ones
// Returns NULL if the index is out of bounds
PATCH_API ModDifficulty_t *PATCH_CALLTYPE ClassicsModData_GetDiff(int iDifficulty);

// Get length of the fixed-size difficulty array
PATCH_API int PATCH_CALLTYPE ClassicsModData_DiffArrayLength(void);

// Reset every difficulty in the array starting from a specific one
// Useful when setting up own list of difficulties
// Example usage:
//    *ClassicsModData_GetDifficulty(0) = ModDifficulty_t(0, "Easy",   "time to relax");
//    *ClassicsModData_GetDifficulty(1) = ModDifficulty_t(1, "Normal", "recommended experience");
//    *ClassicsModData_GetDifficulty(2) = ModDifficulty_t(2, "Hard",   "for the worthy", "gam_bHardDifficulty");
//    ClassicsModData_ClearDiffArray(3); // Clear the rest
PATCH_API void PATCH_CALLTYPE ClassicsModData_ClearDiffArray(int iFromDifficulty = 0);

// Check the difficulty command to see if it's enabled
// If the command isn't set or cannot be found in the shell, it's always active
PATCH_API bool PATCH_CALLTYPE ClassicsModData_IsDiffActive(int iDifficulty);

// Get amount of configured (named) difficulties in the array
// Stops counting difficulties as soon as it encounters a difficulty with no name
PATCH_API int PATCH_CALLTYPE ClassicsModData_CountNamedDiffs(void);

// Find difficulty index by its level
PATCH_API int PATCH_CALLTYPE ClassicsModData_FindDiffByLevel(int iLevel);

#endif // CLASSICSPATCH_IMODDATA_H

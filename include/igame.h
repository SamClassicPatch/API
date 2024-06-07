// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IGAME_H
#define CLASSICSPATCH_IGAME_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

class IClassicsGame
{
public:
  // Hook default fields from vanilla CGame interface
  virtual void HookFields(void) = 0;

  // Check if CGame fields have been hooked
  virtual bool IsHooked(void) = 0;

  // Needs to be set to true after hooking custom fields for CGame interface
  virtual void SetHooked(bool bState) = 0;

  // Get enum index of a gamemode from the array of possible gamemodes
  virtual int GetGameMode(int i) = 0;

  // Console state
  virtual int GetConState(void) = 0;
  virtual void SetConState(int iState) = 0;

  // Computer state
  virtual int GetCompState(void) = 0;
  virtual void SetCompState(int iState) = 0;

  // Active game state
  virtual bool IsGameOn(void) = 0;
  virtual void SetGameState(bool bState) = 0;

  // In-game menu state
  virtual bool IsMenuOn(void) = 0;
  virtual void SetMenuState(bool bState) = 0;

  // State of loading the intro world
  virtual bool GetFirstLoading(void) = 0;
  virtual void SetFirstLoading(bool bState) = 0;

  //================================================================================================//
  // Split-screen configuration modes
  //
  // "iConfiguration" should be set using values from CGame::SplitScreenCfg enum
  //================================================================================================//

  // Mode in the game menu for setting up the game
  virtual int GetMenuSplitCfg(void) = 0;
  virtual void SetMenuSplitCfg(int iConfiguration) = 0;

  // Mode that the game starts in
  virtual int GetStartSplitCfg(void) = 0;
  virtual void SetStartSplitCfg(int iConfiguration) = 0;

  // Mode in the currently active game
  virtual int GetCurrentSplitCfg(void) = 0;
  virtual void SetCurrentSplitCfg(int iConfiguration) = 0;

  //================================================================================================//
  // Local players & player profiles
  //
  // Local players are used for setting up players in split-screen mode using unique player profiles.
  // Indices of local players in the fixed-size array are referred to as "iPlayer".
  //
  // Player profiles are unique customizable characters used for distinguishing local players and
  // different clients on a server. Each profile uses its own save files and controls.
  // Indices of player profiles in the fixed-size array are referred to as "iProfile".
  //================================================================================================//

  // Get maximum amount of local players that can be used in split-screen
  virtual int GetLocalPlayerCount(void) = 0;

  // Get maximum amount of player profiles that the game client supports
  virtual int GetProfileCount(void) = 0;

  // Check if a local player is active during current game
  virtual bool IsLocalPlayerActive(int iPlayer) = 0;

  // Get player profile index associated with a local player
  virtual int GetProfileFromLocalPlayer(int iPlayer) = 0;

  // Index of a player profile used for starting a singleplayer game
  virtual int GetProfileForSP(void) = 0;
  virtual void SetProfileForSP(int iProfile) = 0;

  // Indices of player profiles for modifications via in-game menu
  virtual int GetProfileForMenu(int iPlayer) = 0;
  virtual void SetProfileForMenu(int iPlayer, int iProfile) = 0;

  // Indices of player profiles that will be starting the game
  virtual int GetProfileForStart(int iPlayer) = 0;
  virtual void SetProfileForStart(int iPlayer, int iProfile) = 0;

  // Reset profiles for all local players
  virtual void ResetStartProfiles(void) = 0;

  // Copy indices of menu profiles into start profiles
  virtual void SetStartProfilesFromMenuProfiles(void) = 0;
};

// Get pointer to the game interface
// Returns NULL if Classics Patch hasn't been initialized yet
PATCH_API IClassicsGame *PATCH_CALLTYPE ClassicsGame(void);

#endif // CLASSICSPATCH_IGAME_H

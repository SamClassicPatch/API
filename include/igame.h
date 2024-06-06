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

  // Get amount of available local players
  virtual int GetLocalPlayerCount(void) = 0;

  // Get amount of available player profiles
  virtual int GetProfileCount(void) = 0;

  // Console state
  virtual int GetConState(void) = 0;
  virtual void SetConState(int iState) = 0;

  // Computer state
  virtual int GetCompState(void) = 0;
  virtual void SetCompState(int iState) = 0;

  // Index of a player profile used for starting a singleplayer game
  virtual int GetPlayerForSP(void) = 0;
  virtual void SetPlayerForSP(int iPlayer) = 0;

  // Split-screen configuration mode in the game menu
  virtual int GetMenuSplitCfg(void) = 0;
  virtual void SetMenuSplitCfg(int iConfiguration) = 0;

  // Split-screen configuration mode that the game starts with
  virtual int GetStartSplitCfg(void) = 0;
  virtual void SetStartSplitCfg(int iConfiguration) = 0;

  // Split-screen configuration mode in the currently active game
  virtual int GetCurrentSplitCfg(void) = 0;
  virtual void SetCurrentSplitCfg(int iConfiguration) = 0;

  // Active game state
  virtual bool IsGameOn(void) = 0;
  virtual void SetGameState(bool bState) = 0;

  // In-game menu state
  virtual bool IsMenuOn(void) = 0;
  virtual void SetMenuState(bool bState) = 0;

  // State of loading the intro world
  virtual bool GetFirstLoading(void) = 0;
  virtual void SetFirstLoading(bool bState) = 0;

  // Index of a player profile for modifications via in-game menu
  virtual int GetMenuPlayer(int i) = 0;
  virtual void SetMenuPlayer(int i, int iPlayer) = 0;

  // Indices of player profiles that will be starting the game
  virtual int GetStartPlayer(int i) = 0;
  virtual void SetStartPlayer(int i, int iPlayer) = 0;

  // Reset all start players
  virtual void ResetStartPlayers(void) = 0;

  // Copy menu player indices into start player indices
  virtual void SetStartPlayersFromMenuPlayers(void) = 0;

  // Check if local player is active
  virtual bool IsLocalPlayerActive(int iPlayer) = 0;

  // Get index of a local player
  virtual int GetLocalPlayerIndex(int iPlayer) = 0;
};

// Get pointer to the game interface
// Returns NULL if Classics Patch hasn't been initialized yet
PATCH_API IClassicsGame *PATCH_CALLTYPE ClassicsGame(void);

#endif // CLASSICSPATCH_IGAME_H

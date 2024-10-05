// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_PLUGINEVENTS_H
#define CLASSICSPATCH_PLUGINEVENTS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Various engine classes passed into functions by pointer
class CAnyProjection3D;
class CDrawPort;
class CEntity;
class CEntityEvent;
class CNetworkMessage;
class CPlayerAction;
class CPlayerCharacter;
class CPlayerTarget;
class CTFileName;
class CTString;
class CWorld;

//================================================================================================//
// Defining plugin events
//
// The way custom events can be added to plugins is by manually defining functions in plugin's
// source files and adding pointers to them into specific event interfaces.
//
// When plugin is initialized (started up), a reference to PluginEvents_t structure is passed into
// the function with nullified function pointers in each event interface. These interfaces can be
// used to specify event functions that need to be called by the patch. Unspecified functions are
// ignored and are never called.
//
// When plugin is cleaned up (shut down), all interfaces are automatically cleared out in order to
// prevent the functions from being called. The plugin needs to be reinitialized anew in order to
// refill the events.
//
// IMPORTANT: None of the structures here should ever be defined by plugins! The only way to access
// and specify events is through plugin initialization!
//
// Example usage:
//    CLASSICSPATCH_PLUGIN_STARTUP(CIniConfig &props, PluginEvents_t &events) {
//      // Register plugin functions that need to be called
//      events.m_processing->OnStep = &PluginOnStep;
//      events.m_world->OnWorldLoad = &PluginOnWorldLoad;
//
//      ...other startup code...
//    };
//================================================================================================//

// Generic events for processing actions per in-game tick and per rendered frame
struct IProcessingEvents
{
  // Every simulation tick for synchronizing logic with entities
  void (PATCH_CALLTYPE *OnStep)(void);

  // After rendering the entire game window
  void (PATCH_CALLTYPE *OnFrame)(CDrawPort *pdp);
};

// Events for rendering custom graphics on top of the world and the entire view of the current game
struct IRenderingEvents
{
  // Before drawing the current game
  void (PATCH_CALLTYPE *OnPreDraw)(CDrawPort *pdp);

  // After drawing the current game
  void (PATCH_CALLTYPE *OnPostDraw)(CDrawPort *pdp);

  // After drawing the entire world from a specific perspective
  // penViewer may be NULL in some cases
  void (PATCH_CALLTYPE *OnRenderView)(CWorld &wo, CEntity *penViewer, CAnyProjection3D &apr, CDrawPort *pdp);
};

// Events for catching actions in multiplayer games
struct INetworkEvents
{
  // Upon receiving an extension packet as a server
  // Should return true if the packet was handled and shouldn't be processed by other plugins down the list
  BOOL (PATCH_CALLTYPE *OnServerPacket)(CNetworkMessage &nmMessage, const ULONG ulType);

  // Upon receiving an extension packet as a client
  // Should return true if the packet was handled and shouldn't be processed by other plugins down the list
  BOOL (PATCH_CALLTYPE *OnClientPacket)(CNetworkMessage &nmMessage, const ULONG ulType);

  // Upon adding a new player to the game
  // bLocal is only equal to true when a player of the local client is added, not of other clients
  void (PATCH_CALLTYPE *OnAddPlayer)(CPlayerTarget &plt, BOOL bLocal);

  // Upon removing a player from the game
  // bLocal is only equal to true when a player of the local client is removed, not of other clients
  void (PATCH_CALLTYPE *OnRemovePlayer)(CPlayerTarget &plt, BOOL bLocal);
};

// Events for interacting with specific packets received from clients by the server
struct IPacketEvents
{
  // Upon a player of some client joining the game with some player character
  // Can be used to customize characters of any player to force a specific name, skin and settings
  void (PATCH_CALLTYPE *OnCharacterConnect)(INDEX iClient, CPlayerCharacter &pc);

  // Upon changing character of a specific player of a client
  // Can be used to customize characters of any player to force a specific name, skin and settings
  // If this method returns false, the new character will not be set and all the changes will be discarded,
  // which is only useful when specific (or all) players are banned from modifying their characters mid-game
  //
  // iPlayer can be used in:
  // - CServer::srv_aplbPlayers for CPlayerBuffer (currently set character)
  // - CSessionState::ses_apltPlayers for CPlayerTarget (associated player entity)
  BOOL (PATCH_CALLTYPE *OnCharacterChange)(INDEX iClient, INDEX iPlayer, CPlayerCharacter &pc);

  // Upon receiving actions for a specific player of a client
  //
  // iPlayer can be used in:
  // - CServer::srv_aplbPlayers for CPlayerBuffer (last send action)
  // - CSessionState::ses_apltPlayers for CPlayerTarget (associated player entity)
  // iResent is equal to -1 for normal actions and >=0 for resent actions
  void (PATCH_CALLTYPE *OnPlayerAction)(INDEX iClient, INDEX iPlayer, CPlayerAction &pa, INDEX iResent);

  // Upon receiving a chat message addressed from specific players to other players
  // If this method returns false, it won't be shown in chat as a regular chat message
  // strMessage - chat message that may be modified before being passed into the internal chat command parser
  BOOL (PATCH_CALLTYPE *OnChatMessage)(INDEX iClient, ULONG ulFrom, ULONG ulTo, CTString &strMessage);
};

// Events for catching generic game actions
struct IGameEvents
{
  // After fully loading a specific world and starting a local game on it
  // This method is executed even before adding any local player entities
  void (PATCH_CALLTYPE *OnGameStart)(void);

  // After switching from one level to another (e.g. in a co-op mappack)
  void (PATCH_CALLTYPE *OnChangeLevel)(void);

  // Before stopping a local game, e.g. by shutting down the server or by disconnecting from any
  void (PATCH_CALLTYPE *OnGameStop)(void);

  // After saving a local game on disk
  // fnmSave - path to the .sav file
  void (PATCH_CALLTYPE *OnGameSave)(const CTFileName &fnmSave);

  // After loading a local game from disk
  // fnmSave - path to the .sav file
  void (PATCH_CALLTYPE *OnGameLoad)(const CTFileName &fnmSave);
};

// Events related to recording demos
struct IDemoEvents
{
  // After starting playing a demo from the game menu
  // fnmDemo - path to the .dem file
  void (PATCH_CALLTYPE *OnDemoPlay)(const CTFileName &fnmDemo);

  // After starting recording a demo mid-game
  // fnmDemo - path to the .dem file
  void (PATCH_CALLTYPE *OnDemoStart)(const CTFileName &fnmDemo);

  // After stopping recording a demo mid-game
  void (PATCH_CALLTYPE *OnDemoStop)(void);
};

// Events for manipulating in-game worlds
struct IWorldEvents
{
  // After fully finishing reading and loading a specific world file
  // fnmWorld - path to the .wld file
  //
  // This method can be used to modify contents of the worlds even before calculating their initial state
  // by the server. Unlike IGameEvents::OnGameStart(), this method works even after changing levels but
  // because of modified initial world states, clients will *not* be able to join the server unless they
  // also have this same plugin that is able to modify the initial world state.
  void (PATCH_CALLTYPE *OnWorldLoad)(CWorld *pwo, const CTFileName &fnmWorld);
};

// Events for catching specific actions performed by in-game entities
struct IListenerEvents
{
  // Upon sending any event via CEntity::SendEvent() during entity logic
  // Any sent event after this may be processed via entity's HandleEvent() function
  // or by a wait() block inside a procedure
  void (PATCH_CALLTYPE *OnSendEvent)(CEntity *pen, const CEntityEvent &ee);

  // Upon any player receiving some item via CPlayer::ReceiveItem()
  // If CPlayer::ReceiveItem() isn't defined in the Entities library, this method is never called
  // penPlayer - player entity that received the item
  // ee - item pickup event, e.g. EArmor or EWeaponItem
  // bPickedUp - return value from the actual CPlayer::ReceiveItem() call
  void (PATCH_CALLTYPE *OnReceiveItem)(CEntity *penPlayer, const CEntityEvent &ee, BOOL bPickedUp);

  // Upon calling an internal subautomation within entity logic via CRationalEntity::Call()
  void (PATCH_CALLTYPE *OnCallProcedure)(CEntity *pen, const CEntityEvent &ee);
};

// Events for executing simulation-independent actions every game tick
struct ITimerEvents
{
  // Executed each tick (1/20 of a second in 1.05 and 1.07)
  void (PATCH_CALLTYPE *OnTick)(void);

  // Executed each second
  void (PATCH_CALLTYPE *OnSecond)(void);
};

//================================================================================================//
// Plugin events interface
//
// The main interface that is passed into the plugin on initialization.
// It may only be used for specifying pointers to each existing event interface.
//
// The pointers to individual event interfaces should NOT be changed!
//================================================================================================//

struct PluginEvents_t
{
  IProcessingEvents *const m_processing;
  IRenderingEvents  *const m_rendering;
  INetworkEvents    *const m_network;
  IPacketEvents     *const m_packet;
  IGameEvents       *const m_game;
  IDemoEvents       *const m_demo;
  IWorldEvents      *const m_world;
  IListenerEvents   *const m_listener;
  ITimerEvents      *const m_timer;

  PluginEvents_t();
  ~PluginEvents_t();
};

// Declare plugin event functions
#include "plugineventfunctions.h"

#endif // CLASSICSPATCH_PLUGINEVENTS_H

// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_PLUGINEVENTS_H
#define CLASSICSPATCH_PLUGINEVENTS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

// Register specific plugin events for executing them later on
PATCH_API void PATCH_CALLTYPE ClassicsPlugins_RegisterEvents(class IAbstractEvents *pEvents);

// Unregister specific plugin events to prevent them from being executed
PATCH_API void PATCH_CALLTYPE ClassicsPlugins_UnregisterEvents(class IAbstractEvents *pEvents);

// Types of available plugin events defined below as interfaces
enum EPluginEventType
{
  k_EPluginEventType_Invalid = -1,

  k_EPluginEventType_Processing = 0,
  k_EPluginEventType_Rendering  = 1,
  k_EPluginEventType_Network    = 2,
  k_EPluginEventType_Packet     = 3,
  k_EPluginEventType_Game       = 4,
  k_EPluginEventType_Demo       = 5,
  k_EPluginEventType_World      = 6,
  k_EPluginEventType_Listener   = 7,
  k_EPluginEventType_Timer      = 8,

  k_EPluginEventType_Max,
};

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

// Abstract interface for any plugin events
class IAbstractEvents
{
public:
  // Index of the handler associated with these events in the global container
  int m_iHandler;

  IAbstractEvents() : m_iHandler(-1) {};

  // Destructor with automatic unregistering
  virtual ~IAbstractEvents() {
    Unregister();
  };

  // Type that will be used for placing events in their dedicated container of all event handlers upon registering
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Invalid; };

  inline void Register(void) { ClassicsPlugins_RegisterEvents(this); };
  inline void Unregister(void) { ClassicsPlugins_UnregisterEvents(this); };
};

//================================================================================================//
// Defining plugin events
//
// The way custom events can be added to plugins is by creating interfaces of virtual functions
// and manually defining them in plugin's source files.
//
// The way this works, in simple terms:
//    It is similar to how new Serious Sam entities can be created using external entity packs
//    or even vanilla Entities library. It defines its own virtual tables of functions inside
//    the current library and fills them with pointers to locally defined functions.
//
// A more detailed explanation:
//    When a class with a virtual table is created, it has to either:
//    - Import a virtual table (or some of its functions) from another library
//    - Manually fill it with functions from the current library
//    Since these functions aren't being imported from anywhere, they are left undefined, which
//    makes the compiler display an error about them. By defining own functions within plugins,
//    these virtual tables become unique to each library instead of sharing the table pointing
//    to the same functions, even if it's the same class type.
//================================================================================================//

// Generic events for processing actions per in-game tick and per rendered frame
class IProcessingEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Processing; };

  virtual void OnStep(void); // Every simulation tick for synchronizing logic with entities
  virtual void OnFrame(CDrawPort *pdp); // After rendering the entire game window
};

// Events for rendering custom graphics on top of the world and the entire view of the current game
class IRenderingEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Rendering; };

  virtual void OnPreDraw(CDrawPort *pdp); // Before drawing the current game
  virtual void OnPostDraw(CDrawPort *pdp); // After drawing the current game

  // After drawing the entire world from a specific perspective
  // penViewer may be NULL in some cases
  virtual void OnRenderView(CWorld &wo, CEntity *penViewer, CAnyProjection3D &apr, CDrawPort *pdp);
};

// Events for catching actions in multiplayer games
class INetworkEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Network; };

  // Upon receiving an extension packet as a server
  // Should return true if the packet was handled and shouldn't be processed by other plugins down the list
  virtual BOOL OnServerPacket(CNetworkMessage &nmMessage, const ULONG ulType);

  // Upon receiving an extension packet as a client
  // Should return true if the packet was handled and shouldn't be processed by other plugins down the list
  virtual BOOL OnClientPacket(CNetworkMessage &nmMessage, const ULONG ulType);

  // Upon adding a new player to the game
  // bLocal is only equal to true when a player of the local client is added, not of other clients
  virtual void OnAddPlayer(CPlayerTarget &plt, BOOL bLocal);

  // Upon removing a player from the game
  // bLocal is only equal to true when a player of the local client is removed, not of other clients
  virtual void OnRemovePlayer(CPlayerTarget &plt, BOOL bLocal);
};

// Events for interacting with specific packets received from clients by the server
class IPacketEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Packet; };

  // Upon a player of some client joining the game with some player character
  // Can be used to customize characters of any player to force a specific name, skin and settings
  virtual void OnCharacterConnect(INDEX iClient, CPlayerCharacter &pc);

  // Upon changing character of a specific player of a client
  // Can be used to customize characters of any player to force a specific name, skin and settings
  // If this method returns false, the new character will not be set and all the changes will be discarded,
  // which is only useful when specific (or all) players are banned from modifying their characters mid-game
  //
  // iPlayer can be used in:
  // - CServer::srv_aplbPlayers for CPlayerBuffer (currently set character)
  // - CSessionState::ses_apltPlayers for CPlayerTarget (associated player entity)
  virtual BOOL OnCharacterChange(INDEX iClient, INDEX iPlayer, CPlayerCharacter &pc);

  // Upon receiving actions for a specific player of a client
  //
  // iPlayer can be used in:
  // - CServer::srv_aplbPlayers for CPlayerBuffer (last send action)
  // - CSessionState::ses_apltPlayers for CPlayerTarget (associated player entity)
  // iResent is equal to -1 for normal actions and >=0 for resent actions
  virtual void OnPlayerAction(INDEX iClient, INDEX iPlayer, CPlayerAction &pa, INDEX iResent);

  // Upon receiving a chat message addressed from specific players to other players
  // If this method returns false, it won't be shown in chat as a regular chat message
  // strMessage - chat message that may be modified before being passed into the internal chat command parser
  virtual BOOL OnChatMessage(INDEX iClient, ULONG ulFrom, ULONG ulTo, CTString &strMessage);
};

// Events for catching generic game actions
class IGameEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Game; };

  // After fully loading a specific world and starting a local game on it
  // This method is executed even before adding any local player entities
  virtual void OnGameStart(void);

  // After switching from one level to another (e.g. in a co-op mappack)
  virtual void OnChangeLevel(void);

  // Before stopping a local game, e.g. by shutting down the server or by disconnecting from any
  virtual void OnGameStop(void);

  // After saving a local game on disk
  // fnmSave - path to the .sav file
  virtual void OnGameSave(const CTFileName &fnmSave);

  // After loading a local game from disk
  // fnmSave - path to the .sav file
  virtual void OnGameLoad(const CTFileName &fnmSave);
};

// Events related to recording demos
class IDemoEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Demo; };

  // After starting playing a demo from the game menu
  // fnmDemo - path to the .dem file
  virtual void OnDemoPlay(const CTFileName &fnmDemo);

  // After starting recording a demo mid-game
  // fnmDemo - path to the .dem file
  virtual void OnDemoStart(const CTFileName &fnmDemo);

  // After stopping recording a demo mid-game
  virtual void OnDemoStop(void);
};

// Events for manipulating in-game worlds
class IWorldEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_World; };

  // After fully finishing reading and loading a specific world file
  // fnmWorld - path to the .wld file
  //
  // This method can be used to modify contents of the worlds even before calculating their initial state
  // by the server. Unlike IGameEvents::OnGameStart(), this method works even after changing levels but
  // because of modified initial world states, clients will *not* be able to join the server unless they
  // also have this same plugin that is able to modify the initial world state.
  virtual void OnWorldLoad(CWorld *pwo, const CTFileName &fnmWorld);
};

// Events for catching specific actions performed by in-game entities
class IListenerEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Listener; };

  // Upon sending any event via CEntity::SendEvent() during entity logic
  // Any sent event after this may be processed via entity's HandleEvent() function
  // or by a wait() block inside a procedure
  virtual void OnSendEvent(CEntity *pen, const CEntityEvent &ee);

  // Upon any player receiving some item via CPlayer::ReceiveItem()
  // If CPlayer::ReceiveItem() isn't defined in the Entities library, this method is never called
  // penPlayer - player entity that received the item
  // ee - item pickup event, e.g. EArmor or EWeaponItem
  // bPickedUp - return value from the actual CPlayer::ReceiveItem() call
  virtual void OnReceiveItem(CEntity *penPlayer, const CEntityEvent &ee, BOOL bPickedUp);

  // Upon calling an internal subautomation within entity logic via CRationalEntity::Call()
  virtual void OnCallProcedure(CEntity *pen, const CEntityEvent &ee);
};

// Events for executing simulation-independent actions every game tick
class ITimerEvents : public IAbstractEvents
{
public:
  virtual EPluginEventType GetType(void) { return k_EPluginEventType_Timer; };

  // Executed each tick (1/20 of a second in 1.05 and 1.07)
  virtual void OnTick(void);

  // Executed each second
  virtual void OnSecond(void);
};

#endif // CLASSICSPATCH_PLUGINEVENTS_H

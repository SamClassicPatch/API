// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_PLUGINEVENTFUNCTIONS_H
#define CLASSICSPATCH_PLUGINEVENTFUNCTIONS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"
#include "pluginevents.h"

//================================================================================================//
// Plugin event functions
//
// This list of functions is a convenient way of declaring every single function for plugin event
// interfaces with common names and calling conventions.
//
// The functions listed here should be defined in plugin's source files and then registered upon
// plugin initialization.
//================================================================================================//

// Processing
void PATCH_CALLTYPE IProcessingEvents_OnStep(void);
void PATCH_CALLTYPE IProcessingEvents_OnFrame(CDrawPort *pdp);

// Rendering
void PATCH_CALLTYPE IRenderingEvents_OnPreDraw(CDrawPort *pdp);
void PATCH_CALLTYPE IRenderingEvents_OnPostDraw(CDrawPort *pdp);
void PATCH_CALLTYPE IRenderingEvents_OnRenderView(CWorld &wo, CEntity *penViewer, CAnyProjection3D &apr, CDrawPort *pdp);

// Network
BOOL PATCH_CALLTYPE INetworkEvents_OnServerPacket(CNetworkMessage &nmMessage, const ULONG ulType);
BOOL PATCH_CALLTYPE INetworkEvents_OnClientPacket(CNetworkMessage &nmMessage, const ULONG ulType);
void PATCH_CALLTYPE INetworkEvents_OnAddPlayer(CPlayerTarget &plt, BOOL bLocal);
void PATCH_CALLTYPE INetworkEvents_OnRemovePlayer(CPlayerTarget &plt, BOOL bLocal);

// Packets
void PATCH_CALLTYPE IPacketEvents_OnCharacterConnect(INDEX iClient, CPlayerCharacter &pc);
BOOL PATCH_CALLTYPE IPacketEvents_OnCharacterChange(INDEX iClient, INDEX iPlayer, CPlayerCharacter &pc);
void PATCH_CALLTYPE IPacketEvents_OnPlayerAction(INDEX iClient, INDEX iPlayer, CPlayerAction &pa, INDEX iResent);
BOOL PATCH_CALLTYPE IPacketEvents_OnChatMessage(INDEX iClient, ULONG ulFrom, ULONG ulTo, CTString &strMessage);

// Game
void PATCH_CALLTYPE IGameEvents_OnGameStart(void);
void PATCH_CALLTYPE IGameEvents_OnChangeLevel(void);
void PATCH_CALLTYPE IGameEvents_OnGameStop(void);
void PATCH_CALLTYPE IGameEvents_OnGameSave(const CTFileName &fnmSave);
void PATCH_CALLTYPE IGameEvents_OnGameLoad(const CTFileName &fnmSave);

// Demos
void PATCH_CALLTYPE IDemoEvents_OnDemoPlay(const CTFileName &fnmDemo);
void PATCH_CALLTYPE IDemoEvents_OnDemoStart(const CTFileName &fnmDemo);
void PATCH_CALLTYPE IDemoEvents_OnDemoStop(void);

// Worlds
void PATCH_CALLTYPE IWorldEvents_OnWorldLoad(CWorld *pwo, const CTFileName &fnmWorld);

// Listeners
void PATCH_CALLTYPE IListenerEvents_OnSendEvent(CEntity *pen, const CEntityEvent &ee);
void PATCH_CALLTYPE IListenerEvents_OnReceiveItem(CEntity *penPlayer, const CEntityEvent &ee, BOOL bPickedUp);
void PATCH_CALLTYPE IListenerEvents_OnCallProcedure(CEntity *pen, const CEntityEvent &ee);

// Timer
void PATCH_CALLTYPE ITimerEvents_OnTick(void);
void PATCH_CALLTYPE ITimerEvents_OnSecond(void);

#endif // CLASSICSPATCH_PLUGINEVENTFUNCTIONS_H

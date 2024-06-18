// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_EXTPACKETS_H
#define CLASSICSPATCH_EXTPACKETS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"

//================================================================================================//
// Extension packets
//
// These are custom packets that can be sent over network for implementing new functionality in
// multiplayer games that's always synchronized between clients. This system is built on top of
// internal engine code for receiving networking packets in order to be able to process our own.
//
// These custom extension packets are simply sub-types of a normal network packet under the last
// possible index (63). At the very beginning of the packet an index is written that's used to
// distinguish actual extension packets that can be defined using the abstract base below.
// Custom extension packets need to be processed via plugin events INetworkEvents::OnServerPacket
// (when receiving a packet from a client) and INetworkEvents::OnClientPacket (when receiving a
// packet from a server). For example:
//    BOOL INetworkEvents::OnServerPacket(CNetworkMessage &nmMessage, const ULONG ulType) {
//      // Read and process our custom packet
//      if (ulType == k_EPacketType_MyCustomPacketType) {
//        CMyExtensionPacket pck;
//        pck.Read(nmMessage);
//        pck.Process();
//        return TRUE; // Handled our packet
//      }
//      return FALSE; // Unknown packet, let other plugins handle it
//    };
//
// Built-in extension packets are processed directly by the patch but can also be caught by plugins
// using their type for custom processing, if necessary, albeit not recommended.
//================================================================================================//

// Abstract base for extension packets
class IClassicsExtPacket
{
public:
  // Built-in extension packets
  enum EPacketType {
    // Server to all clients
    k_EPacketType_EntityCreate   =  0, // Create new entity
    k_EPacketType_EntityDelete   =  1, // Delete an entity
    k_EPacketType_EntityCopy     =  2, // Copy an entity
    k_EPacketType_EntityEvent    =  3, // Send event to an entity
    k_EPacketType_EntityItem     =  4, // Receive item by an entity
    k_EPacketType_EntityInit     =  5, // (Re)initialize an entity
    k_EPacketType_EntityTeleport =  6, // Teleport an entity
    k_EPacketType_EntityPosition =  7, // Set position or rotation of an entity
    k_EPacketType_EntityParent   =  8, // Set entity's parent
    k_EPacketType_EntityProp     =  9, // Change property value of an entity
    k_EPacketType_EntityHealth   = 10, // Set entity health
    k_EPacketType_EntityFlags    = 11, // Change various entity flags
    k_EPacketType_EntityMove     = 12, // Set absolute movement speed of an entity
    k_EPacketType_EntityRotate   = 13, // Set absolute rotation speed of an entity
    k_EPacketType_EntityImpulse  = 14, // Give impulse to some entity
    k_EPacketType_EntityDirDmg   = 15, // Inflict damage to some entity
    k_EPacketType_EntityRadDmg   = 16, // Inflict damage at some point
    k_EPacketType_EntityBoxDmg   = 17, // Inflict damage in some area

    k_EPacketType_ChangeLevel    = 18, // Force level change by creating vanilla WorldLink entity, if possible
    k_EPacketType_ChangeWorld    = 19, // Force immediate world change regardless of gameplay
    k_EPacketType_SessionProps   = 20, // Change data in session properties
    k_EPacketType_GameplayExt    = 21, // Change data in gameplay extensions

    // Client to server
    k_EPacketType_C2SReserved = 1000, // Reserved index; do not use

    // Maximum amount of built-in packets, after which custom packets can be added
    k_EPacketType_Max = 2000, // 2000 built-in packets should be enough
  };

  virtual ~IClassicsExtPacket() {};

  // Macro for defining packets of a specific type
  #define EXTPACKET_DEFINEFORTYPE(_Type) \
    virtual EPacketType GetType(void) const { return _Type; }; \
    virtual const char *GetName(void) const { return #_Type; };

  // Get unique extension packet index
  virtual EPacketType GetType(void) const = 0;

  // Internal name of the extension packet type (usually equal to enum type)
  virtual const char *GetName(void) const = 0;

  // Write extension packet before sending it
  // Returns true if the packet has been written successfully and is ready to be sent
  virtual bool Write(class CNetworkMessage &nm) = 0;

  // Read received extension packet before processing it
  virtual void Read(class CNetworkMessage &nm) = 0;

  // Process the packet as a client
  virtual void Process(void) = 0;
};

// Report packet actions to the server
PATCH_API void PATCH_CALLTYPE ClassicsPackets_ServerReport(IClassicsExtPacket *pExtPacket, const char *strFormat, ...);

// Send extension packet from server to all clients
PATCH_API void PATCH_CALLTYPE ClassicsPackets_Send(IClassicsExtPacket *pExtPacket);

#endif // CLASSICSPATCH_EXTPACKETS_H

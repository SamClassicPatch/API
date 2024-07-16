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

class IClassicsExtPacket;

// Report packet actions to the server
PATCH_API void PATCH_CALLTYPE ClassicsPackets_ServerReport(IClassicsExtPacket *pExtPacket, const char *strFormat, ...);

// Send extension packet from server to all clients
PATCH_API void PATCH_CALLTYPE ClassicsPackets_SendToClients(IClassicsExtPacket *pExtPacket);

// Send extension packet from a client to the server
PATCH_API void PATCH_CALLTYPE ClassicsPackets_SendToServer(IClassicsExtPacket *pExtPacket);

// Abstract base for extension packets
class IClassicsExtPacket
{
public:
  virtual void SendToClients(void) { ClassicsPackets_SendToClients(this); };
  virtual void SendToServer(void) { ClassicsPackets_SendToServer(this); };

  // Built-in extension packets
  enum EPacketType {
    // Server to all clients
    k_EPacketType_FirstS2C = 0, // First server-to-client packet index

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

    k_EPacketType_LastS2C = 999, // Last server-to-client packet index

    // Client to server
    k_EPacketType_FirstC2S = 1000, // First client-to-server packet index

    k_EPacketType_LastC2S = 1999, // Last client-to-server packet index

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

  // Needs to be redefined to write specific extension packet data that will be sent
  // Returns true if the packet has been written successfully and is ready to be sent
  virtual bool Write(class CNetworkMessage &nm) { return false; };

  // Needs to be redefined to read received extension packet data as it was written
  virtual void Read(class CNetworkMessage &nm) {};

  // Needs to be redefined to specify logic that will be executed after receiving and reading the packet
  virtual void Process(void) {};
};

//================================================================================================//
// Built-in extension packets
//
// This is an abstract base for built-in extension packets that can be externally instantiated,
// configured and sent.
// This class should *not* be used for defining custom extension packets!
//================================================================================================//

class IClassicsBuiltInExtPacket;

// Retrieve value from a bool property of a built-in extension packet
PATCH_API bool PATCH_CALLTYPE ClassicsPackets_GetBoolProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty);

// Retrieve value from an integer property of a built-in extension packet
PATCH_API int PATCH_CALLTYPE ClassicsPackets_GetIntProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty);

// Retrieve value from a float property of a built-in extension packet
PATCH_API double PATCH_CALLTYPE ClassicsPackets_GetFloatProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty);

// Retrieve value from a string property of a built-in extension packet
PATCH_API const char *PATCH_CALLTYPE ClassicsPackets_GetStringProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty);

// Set value to a bool property of a built-in extension packet
// Returns whether or not the value has been set
PATCH_API bool PATCH_CALLTYPE ClassicsPackets_SetBoolProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty, bool bValue);

// Set value to an integer property of a built-in extension packet
// Returns whether or not the value has been set
PATCH_API bool PATCH_CALLTYPE ClassicsPackets_SetIntProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty, int iValue);

// Set value to a float property of a built-in extension packet
// Returns whether or not the value has been set
PATCH_API bool PATCH_CALLTYPE ClassicsPackets_SetFloatProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty, double fValue);

// Set value to a string property of a built-in extension packet
// Returns whether or not the value has been set
PATCH_API bool PATCH_CALLTYPE ClassicsPackets_SetStringProp(IClassicsBuiltInExtPacket *pExtPacket, const char *strProperty, const char *strValue);

// Abstract base for built-in extension packets that can be externally instantiated, configured and sent
// This class should *not* be used for defining custom extension packets!
class IClassicsBuiltInExtPacket : public IClassicsExtPacket
{
public:
  virtual bool GetBoolProp(const char *strProperty) { return ClassicsPackets_GetBoolProp(this, strProperty); };
  virtual int GetIntProp(const char *strProperty) { return ClassicsPackets_GetIntProp(this, strProperty); };
  virtual double GetFloatProp(const char *strProperty) { return ClassicsPackets_GetFloatProp(this, strProperty); };
  virtual const char *GetStringProp(const char *strProperty) { return ClassicsPackets_GetStringProp(this, strProperty); };

  virtual bool SetBoolProp(const char *strProperty, bool bValue) { return ClassicsPackets_SetBoolProp(this, strProperty, bValue); };
  virtual bool SetIntProp(const char *strProperty, int iValue) { return ClassicsPackets_SetIntProp(this, strProperty, iValue); };
  virtual bool SetFloatProp(const char *strProperty, double fValue) { return ClassicsPackets_SetFloatProp(this, strProperty, fValue); };
  virtual bool SetStringProp(const char *strProperty, const char *strValue) { return ClassicsPackets_SetStringProp(this, strProperty, strValue); };
};

// Create a built-in extension packet from some type and return a pointer to it
// After the packet has been sent or processed, it needs to be manually deleted using ClassicsPackets_Destroy()
PATCH_API IClassicsBuiltInExtPacket *PATCH_CALLTYPE ClassicsPackets_Create(IClassicsExtPacket::EPacketType ePacket);

// Destroy previously created built-in extension packet
PATCH_API void PATCH_CALLTYPE ClassicsPackets_Destroy(IClassicsBuiltInExtPacket *pExtPacket);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsPackets
{
public:
  virtual IClassicsBuiltInExtPacket *Create(IClassicsExtPacket::EPacketType ePacket) { return ClassicsPackets_Create(ePacket); };
  virtual void Destroy(IClassicsBuiltInExtPacket *pExtPacket) { ClassicsPackets_Destroy(pExtPacket); };
};

#endif // CLASSICSPATCH_EXTPACKETS_H

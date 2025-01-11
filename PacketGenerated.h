#pragma once
#include "ContentTypes.h"
#include "FVector.h"
#include "SendBuffer.h"
#include "CRecvBuffer.h"
namespace psh 
{
enum ePacketType : PacketType
{
eLogin_ReqLogin,
eLogin_ResLogin,
eLogin_ReqRegister,
eLogin_ResRegister,
eGame_ReqLogin,
eGame_ResLogin,
eGame_ReqLevelEnter,
eGame_ResLevelEnter,
eGame_ReqChangeComplete,
eGame_ResCreateActor,
eGame_ResChracterDetail,
eGame_ResPlayerDetail,
eGame_ResDestroyActor,
eGame_ResGetCoin,
eGame_ReqMove,
eGame_ResMove,
eGame_ResMoveStop,
eGame_ReqAttack,
eGame_ResAttack,
eGame_ResDraw,
eGame_ResHit,
eGame_ResSpawnItem,
eGame_ResDestroyItem,
eDebug_Game_AttackWhenDead,
eGame_ReqChat,
eGame_ResChat,
eVersion_ReqVersion,
eVersion_ResVersion,
eVersion_ReqGameData,
eVersion_ResGameData,
eDebug_Delay,
None,
};

static void MakeLogin_ReqLogin(SendBuffer& buffer, const psh::ID id, const psh::Password pass)
{
	buffer << ePacketType::eLogin_ReqLogin << id << pass;
}

static void GetLogin_ReqLogin(CRecvBuffer& buffer, psh::ID& id, psh::Password& pass)
{
	buffer >> id >> pass;
}

static void MakeLogin_ResLogin(SendBuffer& buffer, const psh::AccountNo accountNo, const psh::ID id, const eLoginResult result, const psh::SessionKey key)
{
	buffer << ePacketType::eLogin_ResLogin << accountNo << id << result << key;
}

static void GetLogin_ResLogin(CRecvBuffer& buffer, psh::AccountNo& accountNo, psh::ID& id, eLoginResult& result, psh::SessionKey& key)
{
	buffer >> accountNo >> id >> result >> key;
}

static void MakeLogin_ReqRegister(SendBuffer& buffer, const psh::ID id, const psh::Password pass, const psh::Nickname nickname)
{
	buffer << ePacketType::eLogin_ReqRegister << id << pass << nickname;
}

static void GetLogin_ReqRegister(CRecvBuffer& buffer, psh::ID& id, psh::Password& pass, psh::Nickname& nickname)
{
	buffer >> id >> pass >> nickname;
}

static void MakeLogin_ResRegister(SendBuffer& buffer, const psh::AccountNo accountNo, const psh::ID id, const eRegisterResult result)
{
	buffer << ePacketType::eLogin_ResRegister << accountNo << id << result;
}

static void GetLogin_ResRegister(CRecvBuffer& buffer, psh::AccountNo& accountNo, psh::ID& id, eRegisterResult& result)
{
	buffer >> accountNo >> id >> result;
}

static void MakeGame_ReqLogin(SendBuffer& buffer, const psh::AccountNo accountNo, const psh::SessionKey key)
{
	buffer << ePacketType::eGame_ReqLogin << accountNo << key;
}

static void GetGame_ReqLogin(CRecvBuffer& buffer, psh::AccountNo& accountNo, psh::SessionKey& key)
{
	buffer >> accountNo >> key;
}

static void MakeGame_ResLogin(SendBuffer& buffer, const psh::AccountNo accountNo, const bool success)
{
	buffer << ePacketType::eGame_ResLogin << accountNo << success;
}

static void GetGame_ResLogin(CRecvBuffer& buffer, psh::AccountNo& accountNo, bool& success)
{
	buffer >> accountNo >> success;
}

static void MakeGame_ReqLevelEnter(SendBuffer& buffer, const psh::AccountNo accountNo, const psh::ServerType type)
{
	buffer << ePacketType::eGame_ReqLevelEnter << accountNo << type;
}

static void GetGame_ReqLevelEnter(CRecvBuffer& buffer, psh::AccountNo& accountNo, psh::ServerType& type)
{
	buffer >> accountNo >> type;
}

static void MakeGame_ResLevelEnter(SendBuffer& buffer, const psh::AccountNo accountNo, const psh::ObjectID myObject, const ServerType type)
{
	buffer << ePacketType::eGame_ResLevelEnter << accountNo << myObject << type;
}

static void GetGame_ResLevelEnter(CRecvBuffer& buffer, psh::AccountNo& accountNo, psh::ObjectID& myObject, ServerType& type)
{
	buffer >> accountNo >> myObject >> type;
}

static void MakeGame_ReqChangeComplete(SendBuffer& buffer, const psh::AccountNo accountNo)
{
	buffer << ePacketType::eGame_ReqChangeComplete << accountNo;
}

static void GetGame_ReqChangeComplete(CRecvBuffer& buffer, psh::AccountNo& accountNo)
{
	buffer >> accountNo;
}

static void MakeGame_ResCreateActor(SendBuffer& buffer, const psh::ObjectID objectId, const psh::eObjectType group, const char characterIndex, const psh::FVector location, const psh::FVector direction, const bool isSpawn)
{
	buffer << ePacketType::eGame_ResCreateActor << objectId << group << characterIndex << location << direction << isSpawn;
}

static void GetGame_ResCreateActor(CRecvBuffer& buffer, psh::ObjectID& objectId, psh::eObjectType& group, char& characterIndex, psh::FVector& location, psh::FVector& direction, bool& isSpawn)
{
	buffer >> objectId >> group >> characterIndex >> location >> direction >> isSpawn;
}

static void MakeGame_ResChracterDetail(SendBuffer& buffer, const psh::ObjectID objectId, const int hp)
{
	buffer << ePacketType::eGame_ResChracterDetail << objectId << hp;
}

static void GetGame_ResChracterDetail(CRecvBuffer& buffer, psh::ObjectID& objectId, int& hp)
{
	buffer >> objectId >> hp;
}

static void MakeGame_ResPlayerDetail(SendBuffer& buffer, const psh::ObjectID objectId, const psh::Nickname nick)
{
	buffer << ePacketType::eGame_ResPlayerDetail << objectId << nick;
}

static void GetGame_ResPlayerDetail(CRecvBuffer& buffer, psh::ObjectID& objectId, psh::Nickname& nick)
{
	buffer >> objectId >> nick;
}

static void MakeGame_ResDestroyActor(SendBuffer& buffer, const psh::ObjectID objectId, const bool isDead, const char cause)
{
	buffer << ePacketType::eGame_ResDestroyActor << objectId << isDead << cause;
}

static void GetGame_ResDestroyActor(CRecvBuffer& buffer, psh::ObjectID& objectId, bool& isDead, char& cause)
{
	buffer >> objectId >> isDead >> cause;
}

static void MakeGame_ResGetCoin(SendBuffer& buffer, const psh::ObjectID objectId, const uint8 value)
{
	buffer << ePacketType::eGame_ResGetCoin << objectId << value;
}

static void GetGame_ResGetCoin(CRecvBuffer& buffer, psh::ObjectID& objectId, uint8& value)
{
	buffer >> objectId >> value;
}

static void MakeGame_ReqMove(SendBuffer& buffer, const psh::FVector location)
{
	buffer << ePacketType::eGame_ReqMove << location;
}

static void GetGame_ReqMove(CRecvBuffer& buffer, psh::FVector& location)
{
	buffer >> location;
}

static void MakeGame_ResMove(SendBuffer& buffer, const psh::ObjectID objectId, const psh::eObjectType group, const psh::FVector location)
{
	buffer << ePacketType::eGame_ResMove << objectId << group << location;
}

static void GetGame_ResMove(CRecvBuffer& buffer, psh::ObjectID& objectId, psh::eObjectType& group, psh::FVector& location)
{
	buffer >> objectId >> group >> location;
}

static void MakeGame_ResMoveStop(SendBuffer& buffer, const psh::ObjectID objectId, const psh::FVector location)
{
	buffer << ePacketType::eGame_ResMoveStop << objectId << location;
}

static void GetGame_ResMoveStop(CRecvBuffer& buffer, psh::ObjectID& objectId, psh::FVector& location)
{
	buffer >> objectId >> location;
}

static void MakeGame_ReqAttack(SendBuffer& buffer, const char type, const psh::FVector dir)
{
	buffer << ePacketType::eGame_ReqAttack << type << dir;
}

static void GetGame_ReqAttack(CRecvBuffer& buffer, char& type, psh::FVector& dir)
{
	buffer >> type >> dir;
}

static void MakeGame_ResAttack(SendBuffer& buffer, const psh::ObjectID objectId, const char type, const psh::FVector dir)
{
	buffer << ePacketType::eGame_ResAttack << objectId << type << dir;
}

static void GetGame_ResAttack(CRecvBuffer& buffer, psh::ObjectID& objectId, char& type, psh::FVector& dir)
{
	buffer >> objectId >> type >> dir;
}

static void MakeGame_ResDraw(SendBuffer& buffer, const psh::FVector loc)
{
	buffer << ePacketType::eGame_ResDraw << loc;
}

static void GetGame_ResDraw(CRecvBuffer& buffer, psh::FVector& loc)
{
	buffer >> loc;
}

static void MakeGame_ResHit(SendBuffer& buffer, const psh::ObjectID victimId, const psh::ObjectID attackerId, const int hp)
{
	buffer << ePacketType::eGame_ResHit << victimId << attackerId << hp;
}

static void GetGame_ResHit(CRecvBuffer& buffer, psh::ObjectID& victimId, psh::ObjectID& attackerId, int& hp)
{
	buffer >> victimId >> attackerId >> hp;
}

static void MakeGame_ResSpawnItem(SendBuffer& buffer, const psh::ItemID id, const psh::eObjectType group, const char characterIndex, const psh::FVector location)
{
	buffer << ePacketType::eGame_ResSpawnItem << id << group << characterIndex << location;
}

static void GetGame_ResSpawnItem(CRecvBuffer& buffer, psh::ItemID& id, psh::eObjectType& group, char& characterIndex, psh::FVector& location)
{
	buffer >> id >> group >> characterIndex >> location;
}

static void MakeGame_ResDestroyItem(SendBuffer& buffer, const psh::ItemID id)
{
	buffer << ePacketType::eGame_ResDestroyItem << id;
}

static void GetGame_ResDestroyItem(CRecvBuffer& buffer, psh::ItemID& id)
{
	buffer >> id;
}

static void MakeDebug_Game_AttackWhenDead(SendBuffer& buffer, const psh::ObjectID id)
{
	buffer << ePacketType::eDebug_Game_AttackWhenDead << id;
}

static void GetDebug_Game_AttackWhenDead(CRecvBuffer& buffer, psh::ObjectID& id)
{
	buffer >> id;
}

static void MakeGame_ReqChat(SendBuffer& buffer, const String chat)
{
	buffer << ePacketType::eGame_ReqChat << chat;
}

static void GetGame_ReqChat(CRecvBuffer& buffer, String& chat)
{
	buffer >> chat;
}

static void MakeGame_ResChat(SendBuffer& buffer, const psh::ObjectID id, const String chat)
{
	buffer << ePacketType::eGame_ResChat << id << chat;
}

static void GetGame_ResChat(CRecvBuffer& buffer, psh::ObjectID& id, String& chat)
{
	buffer >> id >> chat;
}

static void MakeVersion_ReqVersion(SendBuffer& buffer, const int version)
{
	buffer << ePacketType::eVersion_ReqVersion << version;
}

static void GetVersion_ReqVersion(CRecvBuffer& buffer, int& version)
{
	buffer >> version;
}

static void MakeVersion_ResVersion(SendBuffer& buffer, const int version)
{
	buffer << ePacketType::eVersion_ResVersion << version;
}

static void GetVersion_ResVersion(CRecvBuffer& buffer, int& version)
{
	buffer >> version;
}

static void MakeVersion_ReqGameData(SendBuffer& buffer, const int version)
{
	buffer << ePacketType::eVersion_ReqGameData << version;
}

static void GetVersion_ReqGameData(CRecvBuffer& buffer, int& version)
{
	buffer >> version;
}

static void MakeVersion_ResGameData(SendBuffer& buffer, const int version, const String csv)
{
	buffer << ePacketType::eVersion_ResGameData << version << csv;
}

static void GetVersion_ResGameData(CRecvBuffer& buffer, int& version, String& csv)
{
	buffer >> version >> csv;
}

static void MakeDebug_Delay(SendBuffer& buffer, const int delayMs)
{
	buffer << ePacketType::eDebug_Delay << delayMs;
}

static void GetDebug_Delay(CRecvBuffer& buffer, int& delayMs)
{
	buffer >> delayMs;
}


};

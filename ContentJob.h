#pragma once
#include <PacketGenerated.h>
#include "UnrealTypes.h"
#include "TLSPool.h"
class CRecvBuffer;
namespace psh
{
	class MYLIB_API ContentJob
	{
		USE_TLS_POOL(ContentJob)
	public:
		ePacketType _type;
		SessionID _id;

struct stLogin_ResLogin
{
psh::AccountNo accountNo;
psh::ID id;
eLoginResult result;
psh::SessionKey key;
};
struct stLogin_ResRegister
{
psh::AccountNo accountNo;
psh::ID id;
eRegisterResult result;
};
struct stGame_ResLogin
{
psh::AccountNo accountNo;
bool success;
};
struct stGame_ResLevelEnter
{
psh::AccountNo accountNo;
psh::ObjectID myObject;
ServerType type;
};
struct stGame_ResCreateActor
{
psh::ObjectID objectId;
psh::eObjectType group;
char characterIndex;
psh::FVector location;
psh::FVector direction;
bool isSpawn;
};
struct stGame_ResChracterDetail
{
psh::ObjectID objectId;
int hp;
};
struct stGame_ResPlayerDetail
{
psh::ObjectID objectId;
psh::Nickname nick;
};
struct stGame_ResDestroyActor
{
psh::ObjectID objectId;
bool isDead;
char cause;
};
struct stGame_ResGetCoin
{
psh::ObjectID objectId;
uint8 value;
};
struct stGame_ResMove
{
psh::ObjectID objectId;
psh::eObjectType group;
psh::FVector location;
};
struct stGame_ResMoveStop
{
psh::ObjectID objectId;
psh::FVector location;
};
struct stGame_ResAttack
{
psh::ObjectID objectId;
char type;
psh::FVector dir;
};
struct stGame_ResDraw
{
psh::FVector loc;
};
struct stGame_ResHit
{
psh::ObjectID victimId;
psh::ObjectID attackerId;
int hp;
};
struct stGame_ResSpawnItem
{
psh::ItemID id;
psh::eObjectType group;
char characterIndex;
psh::FVector location;
};
struct stGame_ResDestroyItem
{
psh::ItemID id;
};
struct stGame_ResChat
{
psh::ObjectID id;
String chat;
};
struct stVersion_ResVersion
{
int version;
};
struct stVersion_ResGameData
{
int version;
String csv;
};

union
{
        stLogin_ResLogin Login_ResLoginInfo;
        stLogin_ResRegister Login_ResRegisterInfo;
        stGame_ResLogin Game_ResLoginInfo;
        stGame_ResLevelEnter Game_ResLevelEnterInfo;
        stGame_ResCreateActor Game_ResCreateActorInfo;
        stGame_ResChracterDetail Game_ResChracterDetailInfo;
        stGame_ResPlayerDetail Game_ResPlayerDetailInfo;
        stGame_ResDestroyActor Game_ResDestroyActorInfo;
        stGame_ResGetCoin Game_ResGetCoinInfo;
        stGame_ResMove Game_ResMoveInfo;
        stGame_ResMoveStop Game_ResMoveStopInfo;
        stGame_ResAttack Game_ResAttackInfo;
        stGame_ResDraw Game_ResDrawInfo;
        stGame_ResHit Game_ResHitInfo;
        stGame_ResSpawnItem Game_ResSpawnItemInfo;
        stGame_ResDestroyItem Game_ResDestroyItemInfo;
        stGame_ResChat Game_ResChatInfo;
        stVersion_ResVersion Version_ResVersionInfo;
        stVersion_ResGameData Version_ResGameDataInfo;
};

		void Free();
		static int64 GetPoolAllocatedSize() { return _pool.AllocatedCount(); }
		static ContentJob* Alloc() 
		{
			const auto ret = _pool.Alloc();
			ret->_type = psh::None;
			return ret;
		}
	private:
		ContentJob() : _type(psh::None), _id() 	{}
        ~ContentJob() {}
		static TlsPool<ContentJob, 0> _pool;
	};
}
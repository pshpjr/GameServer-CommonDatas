#pragma once

// 헤더 파일 포함
#include <codecvt>
#include <memory>
#include "Types.h"
#include "SessionTypes.h"
#include "GroupTypes.h"
#include "NameString.h"

#if defined(PSH_SERVER) || defined(PSH_CLIENT)
#define UENUM(type)
using uint8 = psh::uint8;
#endif

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Item,
    attackBuff,
    defenceBuff,
    potion
};

namespace psh
{
    // 전방 선언 (Forward Declarations)
    template <size_t L> class staticWString;
    template <size_t L> class staticString;

    // 열거형 (Enumerations)
    enum class ServerType : uint8
    {
        Village,
        Easy,
        Hard,
        Pvp,
        End
    };

    enum class ItemType : uint8
    {
        Gem,
        // 더 많은 항목 추가 예정
    };

    enum class eLoginResult : uint8
    {
        LoginSuccess,
        DuplicateLogin,
        WrongPassword,
        InvalidId
    };

    enum class eRegisterResult : uint8
    {
        RegisterSuccess,
        DuplicateID,
        DuplicateNick
    };

    enum class eObjectType : uint16
    {
        Player = 1 << 0,
        Monster = 1 << 1,
        Item = 1 << 2,
        Object = 1 << 3
    };

    // 타입 정의 및 상수 (Typedefs and Constants)
    using ID = staticWString<12>;
    using Password = staticWString<12>;
    using Nickname = staticWString<20>;
    using PacketType = uint16;
    using AccountNo = int64;
    using ObjectID = int32;
    using ItemID = int32;
    using SessionKey = staticString<64>;
    //0~3: 플레이어, 4~15:몬스터 , 16: 아이템
    using TemplateID = char;
    using SkillID = char;

    // 함수 (Functions)
    inline bool floatCompare(float f1, float f2, const float offset)
    {
        if (f1 > f2)
        {
            std::swap(f1, f2);
        }
        return (f2 - f1 <= offset);
    }

    // 템플릿 (Templates)
    template <class T>
    using shared = std::shared_ptr<T>;

    template <class T, class D = std::default_delete<T>>
    using unique = std::unique_ptr<T, D>;
}
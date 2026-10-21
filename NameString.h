#pragma once
#include <cstring>
#include <algorithm>
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include "Utf16.h"

namespace psh
{
    template <size_t LENGTH>
    class staticWString
    {
        static constexpr char NAME_LENGTH = LENGTH;
        static_assert(LENGTH <= SCHAR_MAX);
    public:
        staticWString();
        explicit staticWString(const std::wstring& nameString);
        explicit staticWString(const char* nameString);
        ~staticWString() = default;
        staticWString(const staticWString& other) = default;
        staticWString(staticWString&& other) noexcept = default;
        staticWString& operator=(const staticWString& other) = default;
        staticWString& operator=(staticWString&& other) noexcept = default;

        bool operator==(const staticWString& other) const
        {
            return wcscmp(_name.data(), other._name.data()) == 0;
        }

        bool operator!=(const staticWString& other) const
        {
            return !(*this == other);
        }

        String ToString() const noexcept
        {
            return std::wstring(_name.data(), LENGTH);
        }

        LPCWSTR operator*() const
        {
            return _name.data();
        }
    private:
        std::array<wchar_t, LENGTH + 1> _name{};

        template <size_t L>
        friend SendBuffer& operator<<(SendBuffer& buffer, const staticWString<L>& name);

        template <size_t L>
        friend RecvBuffer& operator>>(RecvBuffer& buffer, staticWString<L>& name);
    };

    template <size_t LENGTH>
    staticWString<LENGTH>::staticWString()
    {
    }

    template <size_t LENGTH>
    staticWString<LENGTH>::staticWString(const std::wstring& nameString)
    {
        if (util::WideToUtf16(nameString).size() > LENGTH)
            throw std::invalid_argument("Invalid UTF-16 name length");
        size_t copyLen = std::min<size_t>( nameString.length(), static_cast<size_t>(LENGTH));
        std::copy_n(nameString.data(), copyLen, _name.data());
    }

    template<size_t LENGTH>
    inline staticWString<LENGTH>::staticWString(const char* nameString)
    {
        auto encoded = util::WideToUtf16(util::SToW(nameString));
        if (encoded.size() > LENGTH)
        {
            encoded.resize(LENGTH);
            // 고정 길이 경계에서 surrogate pair를 반으로 자르지 않는다.
            if (!encoded.empty() && encoded.back() >= 0xD800 && encoded.back() <= 0xDBFF)
                encoded.pop_back();
        }
        const auto decoded = util::Utf16ToWide(encoded);
        std::copy(decoded.begin(), decoded.end(), _name.data());
    }
}

namespace psh
{
    template <size_t LENGTH>
    SendBuffer& operator<<(SendBuffer& buffer, const staticWString<LENGTH>& name)
    {
        auto encoded = util::WideToUtf16(name._name.data());
        encoded.resize(LENGTH, u'\0');
        buffer.SetUtf16(encoded);
        return buffer;
    }

    template <size_t LENGTH>
    RecvBuffer& operator>>(RecvBuffer& buffer, staticWString<LENGTH>& name)
    {
        buffer.GetWstr(name._name.data(), LENGTH);
        return buffer;
    }
}

namespace psh
{
    template <size_t LENGTH>
    class staticString
    {
        static constexpr char NAME_LENGTH = LENGTH;

    public:
        staticString();
        explicit staticString(const std::string& nameString);
        ~staticString() = default;
        staticString(const staticString& other) = default;
        staticString(staticString&& other) noexcept = default;
        staticString& operator=(const staticString& other) = default;
        staticString& operator=(staticString&& other) noexcept = default;

        bool operator==(const staticString& other) const
        {
            return strcmp(_name, other._name) == 0;
        }

        bool operator!=(const staticString& other) const
        {
            return !(*this == other);
        }

        std::string ToString() const noexcept
        {
            return std::string(_name.data(), LENGTH);
        }

    private:
        std::array<char, LENGTH + 1> _name = {};

        template <size_t L>
        friend SendBuffer& operator<<(SendBuffer& buffer, const staticString<L>& name);

        template <size_t L>
        friend RecvBuffer& operator>>(RecvBuffer& buffer, staticString<L>& name);
    };

    template <size_t LENGTH>
    staticString<LENGTH>::staticString()
    {
    }

    template <size_t LENGTH>
    staticString<LENGTH>::staticString(const std::string& nameString)
    {
        size_t copyLen = std::min<size_t>(nameString.length(), static_cast<size_t>(LENGTH));
        std::copy_n(nameString.data(), copyLen, _name.data());
    }
}

namespace psh
{
    template <size_t L>
    SendBuffer& operator<<(SendBuffer& buffer, const staticString<L>& name)
    {
        buffer.SetCstr(name._name.data(), L);
        return buffer;
    }

    template <size_t L>
    RecvBuffer& operator>>(RecvBuffer& buffer, staticString<L>& name)
    {
        buffer.GetCstr(name._name.data(), L);
        return buffer;
    }
}

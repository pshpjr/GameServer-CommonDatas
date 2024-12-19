#pragma once
#include <cstring>
#include <algorithm>
#include "CRecvBuffer.h"
#include "SendBuffer.h"

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
        friend CRecvBuffer& operator>>(CRecvBuffer& buffer, staticWString<L>& name);
    };

    template <size_t LENGTH>
    staticWString<LENGTH>::staticWString()
    {
    }

    template <size_t LENGTH>
    staticWString<LENGTH>::staticWString(const std::wstring& nameString)
    {
        ASSERT_CRASH(nameString.size() <= LENGTH, "Invalid Name Length");
        size_t copyLen = std::min<size_t>( nameString.length(), static_cast<size_t>(LENGTH));
        wcsncpy_s(_name.data(), LENGTH, nameString.c_str(), copyLen);
    }

    template<size_t LENGTH>
    inline staticWString<LENGTH>::staticWString(const char* nameString)
    {
        size_t copyLen = std::min<size_t>( strlen(nameString), static_cast<size_t>(LENGTH));

        mbstowcs_s(nullptr, _name.data(), LENGTH, nameString, copyLen);

    }
}

namespace psh
{
    template <size_t LENGTH>
    SendBuffer& operator<<(SendBuffer& buffer, const staticWString<LENGTH>& name)
    {
        buffer.SetWstr(name._name.data(), LENGTH);
        return buffer;
    }

    template <size_t LENGTH>
    CRecvBuffer& operator>>(CRecvBuffer& buffer, staticWString<LENGTH>& name)
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
        friend CRecvBuffer& operator>>(CRecvBuffer& buffer, staticString<L>& name);
    };

    template <size_t LENGTH>
    staticString<LENGTH>::staticString()
    {
    }

    template <size_t LENGTH>
    staticString<LENGTH>::staticString(const std::string& nameString)
    {
        size_t copyLen = std::min<size_t>(nameString.length(), static_cast<size_t>(LENGTH));
        strncpy_s(_name.data(), LENGTH, nameString.c_str(), copyLen);
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
    CRecvBuffer& operator>>(CRecvBuffer& buffer, staticString<L>& name)
    {
        buffer.GetCstr(name._name.data(), L);
        return buffer;
    }
}

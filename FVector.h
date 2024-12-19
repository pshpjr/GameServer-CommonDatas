#pragma once
#include <algorithm>
#include <iostream>
#include "CRecvBuffer.h"
#include "SendBuffer.h"

#include <cmath>

namespace psh
{
    struct FVector
    {
        float X;
        float Y;

        friend std::ostream& operator<<(std::ostream &os,const FVector &vec);

        FVector operator+(const FVector& rhs) const
        {
            return FVector{X + rhs.X, Y + rhs.Y};
        }

        FVector& operator+=(const FVector& rhs)
        {
            X += rhs.X;
            Y += rhs.Y;
            return *this;
        }

        FVector operator-(const FVector& rhs) const
        {
            return FVector{X - rhs.X, Y - rhs.Y};
        }

        FVector& operator-=(const FVector& rhs)
        {
            X -= rhs.X;
            Y -= rhs.Y;
            return *this;
        }

        FVector operator*(const float scalar) const
        {
            return FVector{X * scalar, Y * scalar};
        }

        FVector operator/(const float scalar) const
        {
            return FVector{X / scalar, Y / scalar};
        }

        friend bool operator<(const FVector& lhs, const FVector& rhs)
        {
            if (lhs.X < rhs.X)
            {
                return true;
            }
            if (rhs.X < lhs.X)
            {
                return false;
            }
            if (lhs.Y < rhs.Y)
            {
                return true;
            }
            if (rhs.Y < lhs.Y)
            {
                return false;
            }
            return false;
        }

        friend bool operator <=(const FVector& lhs, const FVector& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const FVector& lhs, const FVector& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator>=(const FVector& lhs, const FVector& rhs)
        {
            return !(lhs < rhs);
        }

        friend bool operator==(const FVector& lhs, const FVector& rhs)
        {
            return lhs.X == rhs.X
                   && lhs.Y == rhs.Y;
        }

        friend bool operator!=(const FVector& lhs, const FVector& rhs)
        {
            return !(lhs == rhs);
        }

        float Size() const
        {
            return std::sqrt(X * X + Y * Y);
        }

        FVector Normalize() const
        {
            return {X / Size(), Y / Size()};
        }
    };

    inline FVector Ceil(FVector target, const float value)
    {
        if (target.X < value)
        {
            target.X = value;
        }
        if (target.Y < value)
        {
            target.Y = value;
        }
        return target;
    }

    inline FVector Floor(FVector target, const float value)
    {
        if (target.X > value)
        {
            target.X = value;
        }
        if (target.Y > value)
        {
            target.Y = value;
        }
        return target;
    }

    inline FVector Clamp(const FVector target, const float min, const float max)
    {
        return FVector(std::clamp(target.X, min, max), std::clamp(target.Y, min, max));
    }

    inline FVector Rotate(const FVector vector, const FVector Rotation, const FVector origin) noexcept
    {
        const float theta = std::atan2(Rotation.X, Rotation.Y);
        return FVector(
            (vector.Y - origin.Y) * sin(theta) + (vector.X - origin.X) * cos(theta) + origin.X
            , (vector.Y - origin.Y) * cos(theta) - (vector.X - origin.X) * sin(theta) + origin.Y);
    }

    inline FVector ReverseRotate(const FVector vector, const FVector Rotation, const FVector origin) noexcept
    {
        const float theta = -1* std::atan2(Rotation.X, Rotation.Y);
        return FVector(
            (vector.Y - origin.Y) * sin(theta) + (vector.X - origin.X) * cos(theta) + origin.X
            , (vector.Y - origin.Y) * cos(theta) - (vector.X - origin.X) * sin(theta) + origin.Y);
    }

    inline float Cross(const FVector lhs, const FVector rhs)
    {
        return lhs.Y * rhs.X - lhs.X * rhs.Y;
    }

    inline float CCW(const FVector v1, const FVector v2, const FVector v3)
    {
        auto vec1 = v2 - v1;
        auto vec2 = v3 - v2;

        return Cross(vec1, vec2);
    }

    inline float Distance(const FVector v1, const FVector v2)
    {
        return static_cast<float>(sqrt(pow(v1.X - v2.X, 2) + pow(v1.Y - v2.Y, 2)));
    }

    inline SendBuffer& operator <<(SendBuffer& buffer, const FVector& data)
    {
        buffer << data.X << data.Y;
        return buffer;
    }

    inline CRecvBuffer& operator >>(CRecvBuffer& buffer, FVector& data)
    {
        buffer >> data.X >> data.Y;
        return buffer;
    }

    inline std::ostream& operator<<(std::ostream &os,const FVector &vec)
    {
        os << "Point(" << vec.X << ", " << vec.Y << ")";
        return os;
    }


}

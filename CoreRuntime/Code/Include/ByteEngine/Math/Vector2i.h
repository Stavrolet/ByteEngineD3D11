#pragma once

#include <cassert>

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Primitives.h"

namespace ByteEngine::Math
{
    struct Vector2;
    struct Vector3;

    struct BYTEENGINE_API Vector2i
    {
        union
        {
            struct
            {
                int32 x;
                int32 y;
            };

            struct
            {
                int32 width;
                int32 height;
            };

            int32 data[2];
        };

        constexpr Vector2i(int32 xy = 0)
            : x(xy), y(xy)
        { }

        constexpr Vector2i(int32 x, int32 y)
            : x(x), y(y) 
        { }

        static constexpr Vector2i Zero() { return Vector2i(0); }
        static constexpr Vector2i One() { return Vector2i(1); }
        static constexpr Vector2i Up() { return Vector2i(0, 1); }
        static constexpr Vector2i Down() { return Vector2i(0, -1); }
        static constexpr Vector2i Left() { return Vector2i(-1, 0); }
        static constexpr Vector2i Right() { return Vector2i(1, 0); }

        constexpr Vector2i operator+() const { return *this; }
        constexpr Vector2i operator-() const { return Vector2i(-x, -y); }

        constexpr Vector2i operator+(Vector2i a) const { return Vector2i(a.x + x, a.y + y); }
        constexpr Vector2i operator+(int32 a) const { return Vector2i(a + x, a + y); }
        constexpr Vector2i operator-(Vector2i a) const { return Vector2i(x - a.x, y - a.y); }
        constexpr Vector2i operator-(int32 a) const { return Vector2i(x - a, y - a); }

        constexpr Vector2i& operator+=(Vector2i a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr Vector2i& operator+=(int32 a)
        {
            x += a;
            y += a;
            return *this;
        }

        constexpr Vector2i& operator-=(Vector2i a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr Vector2i& operator-=(int32 a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        constexpr Vector2i operator*(Vector2i a) const { return Vector2i(x * a.x, y * a.y); }
        constexpr Vector2i operator*(int32 s) const { return Vector2i(x * s, y * s); }
        friend constexpr Vector2i operator*(int32 s, Vector2i v) { return v * s; }

        constexpr Vector2i& operator*=(Vector2i a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr Vector2i& operator*=(int32 s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vector2i operator/(Vector2i a) const { return Vector2i(x / a.x, y / a.y); }
        constexpr Vector2i operator/(int32 s) const { return Vector2i(x / s, y / s); }

        constexpr Vector2i& operator/=(Vector2i a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr Vector2i& operator/=(int32 s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        constexpr bool operator==(Vector2i other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(Vector2i other) const { return !(*this == other); }

        int32& operator[](int32 index)
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        int32 operator[](int32 index) const
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        operator Vector2() const;
        operator Vector3() const;
    };
}
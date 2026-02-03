#pragma once

#include "Math/Math.h"

namespace ByteEngine::Math
{
    struct Vector2
    {
        union
        {
            struct
            {
                float x;
                float y;
            };

            struct
            {
                float u;
                float v;
            };

            float data[2];
        };

        explicit constexpr Vector2(float xy = 0.0f)
            : x(xy), y(xy)
        { }

        constexpr Vector2(float x, float y)
            : x(x), y(y)
        { }

        float Length() const { return Math::Sqrt(x * x + y * y); }
        constexpr float LengthSquared() const { return x * x + y * y; }

        void Normalize();
        Vector2 Normalized() const;
        constexpr bool IsNormalized() const { return Math::IsEqualApproximetly(1.0f, LengthSquared(), Math::UnitSizeEpsilon); }

        constexpr void RotateBy(RadianF angle)
        {
            float sin, cos;
            Math::SinCos(sin, cos, angle);
            float oldX = x;
            x = oldX * cos - y * sin;
            y = oldX * sin + y * cos;
        }

        constexpr Vector2 RotatedBy(RadianF angle) const
        {
            Vector2 copy = *this;
            copy.RotateBy(angle);
            return copy;
        }

        void LimitLength(float maxLength = 1.0f);

        static RadianF AngleBetween(Vector2 from, Vector2 to);
        static RadianF UnsigedAngleBetween(Vector2 from, Vector2 to);

        static float Distcance(Vector2 a, Vector2 b) { return Math::Sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }
        static constexpr float DistcanceSquared(Vector2 a, Vector2 b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }

        static Vector2 Direction(Vector2 from, Vector2 to);
        static RadianF DirectionAngle(Vector2 from, Vector2 to);

        static constexpr float Cross(Vector2 a, Vector2 b) { return a.x * b.y - a.y * b.x; }
        static constexpr float Dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }

        static constexpr Vector2 FromAngle(RadianF angle)
        {
            Vector2 vec;
            Math::SinCos(vec.x, vec.y, angle);
            return vec;
        }

        static constexpr Vector2 FromAngle(RadianF angle, float length) { return FromAngle(angle) * length; }

        static constexpr Vector2 Lerp(Vector2 from, Vector2 to, float t) { return from + (to - from) * t; }
        static constexpr Vector2 LerpClamped(Vector2 from, Vector2 to, float t) { return from + (to - from) * Math::Clamp(t); } 
        static Vector2 Slerp(Vector2 from, Vector2 to, float t);
        static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDelta);

        static constexpr Vector2 Project(Vector2 vec, Vector2 projectOnto) { return projectOnto * (Dot(vec, projectOnto) / projectOnto.LengthSquared()); }
        static constexpr Vector2 ProjectNormalized(Vector2 vec, Vector2 projectOnto) { return projectOnto * Dot(vec, projectOnto); }

        static constexpr Vector2 Reflect(Vector2 vec, Vector2 normal) { return vec - 2.0f * Dot(vec, normal) * normal; }

        static constexpr bool IsEqualApproximetly(Vector2 a, Vector2 b) { return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y); }

        constexpr Vector2 operator+() const { return Vector2(+x, +y); }
        constexpr Vector2 operator-() const { return Vector2(-x, -y); }

        constexpr Vector2 operator+(Vector2 a) const { return Vector2(a.x + x, a.y + y); }
        constexpr Vector2 operator+(float a) const { return Vector2(a + x, a + y); }
        constexpr Vector2 operator-(Vector2 a) const { return Vector2(x - a.x, y - a.y); }
        constexpr Vector2 operator-(float a) const { return Vector2(x - a, y - a); }

        constexpr Vector2& operator+=(Vector2 a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr Vector2& operator+=(float a)
        {
            x += a;
            y += a;
            return *this;
        }

        constexpr Vector2& operator-=(Vector2 a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr Vector2& operator-=(float a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        constexpr Vector2 operator*(Vector2 a) const { return Vector2(x * a.x, y * a.y); }
        constexpr Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
        friend constexpr Vector2 operator*(float s, Vector2 v) { return v * s; }

        constexpr Vector2& operator*=(Vector2 a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr Vector2& operator*=(float s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vector2 operator/(Vector2 a) const { return Vector2(x / a.x, y / a.y); }
        constexpr Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

        constexpr Vector2& operator/=(Vector2 a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr Vector2& operator/=(float s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        constexpr bool operator==(Vector2 other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(Vector2 other) const { return !(*this == other); }

        float& operator[](int32 index) 
        {
            assert(index >= 0 && index < 2);
            return data[index]; 
        }

        float operator[](int32 index) const 
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }
    };

    static inline constexpr Vector2 Zero(0.0f);
    static inline constexpr Vector2 One(1.0f);
    static inline constexpr Vector2 Up(0.0f, 1.0f);
    static inline constexpr Vector2 Down(0.0f, -1.0f);
    static inline constexpr Vector2 Left(-1.0f, 0.0f);
    static inline constexpr Vector2 Right(1.0f, 0.0f);
}
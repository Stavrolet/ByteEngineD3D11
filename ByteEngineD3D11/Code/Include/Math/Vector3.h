#pragma once

#include "Math/Math.h"

namespace ByteEngine::Math
{
    struct Vector3
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };

            struct
            {
                float r;
                float g;
                float b;
            };

            float data[3];
        };

        explicit constexpr Vector3(float xyz = 0.0f)
            : x(xyz), y(xyz), z(xyz)
        { }

        constexpr Vector3(float x, float y, float z)
            : x(x), y(y), z(z)
        { }

        float Length() const;
        constexpr float LengthSquared() const { return x * x + y * y + z * z; }

        void Normalize();
        Vector3 Normalized() const;
        constexpr bool IsNormalized() const { return Math::IsEqualApproximetly(1.0f, LengthSquared(), Math::UnitSizeEpsilon); }

        void LimitLength(float maxLength = 1.0f);

        static RadianF AngleBetween(Vector3 from, Vector3 to, Vector3 rotationAxis);
        static RadianF UnsigedAngleBetween(Vector3 from, Vector3 to);

        static float Distcance(Vector3 a, Vector3 b);
        static constexpr float DistcanceSquared(Vector3 a, Vector3 b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector3 Direction(Vector3 from, Vector3 to);

        static constexpr Vector3 Cross(Vector3 a, Vector3 b)
        {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static constexpr float Dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        static constexpr Vector3 Lerp(Vector3 from, Vector3 to, float t) { return from + (to - from) * t; }
        static constexpr Vector3 LerpClamped(Vector3 from, Vector3 to, float t) { return from + (to - from) * Math::Clamp(t); }
        static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDelta);
        
        static constexpr Vector3 Project(Vector3 vec, Vector3 projectOnto) { return projectOnto * (Dot(vec, projectOnto) / projectOnto.LengthSquared()); }
        static constexpr Vector3 ProjectNormalized(Vector3 vec, Vector3 projectOnto) { return projectOnto * Dot(vec, projectOnto); }

        static constexpr Vector3 Reflect(Vector3 vec, Vector3 normal) { return vec - 2.0f * Dot(vec, normal) * normal; }

        static constexpr bool IsEqualApproximetly(Vector3 a, Vector3 b) 
        {
            return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y) && Math::IsEqualApproximetly(a.z, b.z); 
        }

        constexpr Vector3 operator+() const { return Vector3(+x, +y, +z); }
        constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }

        constexpr Vector3 operator+(Vector3 a) const { return Vector3(a.x + x, a.y + y, a.z + z); }
        constexpr Vector3 operator+(float s) const { return Vector3(s + x, s + y, s + z); }
        constexpr Vector3 operator-(Vector3 a) const { return Vector3(x - a.x, y - a.y, z - a.z); }
        constexpr Vector3 operator-(float s) const { return Vector3(x - s, y - s, z - s); }

        constexpr Vector3& operator+=(Vector3 a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr Vector3& operator+=(float s)
        {
            x += s;
            y += s;
            z += s;
            return *this;
        }

        constexpr Vector3& operator-=(Vector3 a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr Vector3& operator-=(float a)
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        constexpr Vector3 operator*(Vector3 a) const { return Vector3(x * a.x, y * a.y, z * a.z); }
        constexpr Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
        friend constexpr Vector3 operator*(float s, Vector3 v) { return v * s; }

        constexpr Vector3& operator*=(Vector3 a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr Vector3& operator*=(float s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        constexpr Vector3 operator/(Vector3 a) const { return Vector3(x / a.x, y / a.y, z / a.z); }
        constexpr Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

        constexpr Vector3& operator/=(Vector3 a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr Vector3& operator/=(float s)
        {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        constexpr bool operator==(Vector3 other) const { return x == other.x && y == other.y && z == other.z; }
        constexpr bool operator!=(Vector3 other) const { return !(*this == other); }

        float& operator[](int32 index)
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        float operator[](int32 index) const
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Left;
        static const Vector3 Right;
        static const Vector3 Forward;
        static const Vector3 Back;
    };

    inline constexpr Vector3 Vector3::Zero(0.0f);
    inline constexpr Vector3 Vector3::One(1.0f);
    inline constexpr Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
    inline constexpr Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
    inline constexpr Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
    inline constexpr Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
    inline constexpr Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
    inline constexpr Vector3 Vector3::Back(0.0f, 0.0f, -1.0f);
}
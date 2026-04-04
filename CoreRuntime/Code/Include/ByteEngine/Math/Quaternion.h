#pragma once

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    struct Quaternion
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };

            float data[4];
        };

        explicit constexpr Quaternion(float xyz = 0.0f, float w = 1.0f)
            : x(xyz), y(xyz), z(xyz), w(w)
        { }

        constexpr Quaternion(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        { }

        constexpr Quaternion(const float arr[4])
            : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
        { }

        BYTEENGINE_API [[nodiscard]] float Length() const;
        [[nodiscard]] constexpr float LengthSquared() const { return x * x + y * y + z * z + w * w; }

        BYTEENGINE_API void Normalize();
        BYTEENGINE_API [[nodiscard]] Quaternion Normalized() const;
        [[nodiscard]] bool IsNormalized() const { return Math::IsEqualApproximetly(LengthSquared(), 1.0f, Math::UnitSizeEpsilon); }

        void Inverse()
        {
            assert(IsNormalized());
            x = -x;
            y = -y;
            z = -z;
        }

        [[nodiscard]] Quaternion Inversed() const
        {
            Quaternion copy = *this;
            copy.Inverse();
            return copy;
        }

        BYTEENGINE_API [[nodiscard]] Vector3F GetEuler();
        BYTEENGINE_API [[nodiscard]] Vector3F GetEuler() const;
        BYTEENGINE_API [[nodiscard]] Vector3F GetEulerInDegrees() const;

        // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::get_axis
        BYTEENGINE_API [[nodiscard]] Vector3F GetAxis() const;
        BYTEENGINE_API [[nodiscard]] RadianF GetAngle() const;

        [[nodiscard]] constexpr static float Dot(Quaternion a, Quaternion b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        BYTEENGINE_API [[nodiscard]] static RadianF AngleBetween(Quaternion a, Quaternion b);

        BYTEENGINE_API [[nodiscard]] static Quaternion FromAngleAxis(RadianF angle, Vector3F axis);
        BYTEENGINE_API [[nodiscard]] static Quaternion FromAngleAxis(DegreeF angle, Vector3F axis);

        BYTEENGINE_API [[nodiscard]] static Quaternion FromEuler(RadianF pitch, RadianF yaw, RadianF roll);
        BYTEENGINE_API [[nodiscard]] static Quaternion FromEuler(DegreeF pitch, DegreeF yaw, DegreeF roll);

        BYTEENGINE_API [[nodiscard]] static Quaternion FromLookDirection(Vector3F direction, Vector3F worldUp = Vector3F::Up());
        BYTEENGINE_API [[nodiscard]] static Quaternion FromToRotation(Vector3F from, Vector3F target);

        BYTEENGINE_API [[nodiscard]] static Quaternion SlerpUnclamped(Quaternion from, Quaternion to, float t);
        BYTEENGINE_API [[nodiscard]] static Quaternion Slerp(Quaternion from, Quaternion to, float t);

        [[nodiscard]] constexpr Quaternion operator+() const { return Quaternion(+x, +y, +z, +w); }
        [[nodiscard]] constexpr Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

        constexpr Quaternion operator+(Quaternion q) const { return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w); }
        constexpr Quaternion operator-(Quaternion q) const { return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w); }

        constexpr Quaternion& operator+=(Quaternion q)
        {
            *this = *this + q;
            return *this;
        }

        constexpr Quaternion& operator-=(Quaternion q)
        {
            *this = *this - q;
            return *this;
        }

        constexpr Quaternion operator*(Quaternion q) const
        {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }

        constexpr Quaternion& operator*=(Quaternion q)
        {
            *this = *this * q;
            return *this;
        }

        constexpr bool operator==(Quaternion other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(Quaternion other) const { return !(*this == other); }

        float& operator[](int32 index)
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        float operator[](int32 index) const
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        [[nodiscard]] constexpr explicit operator Vector3F() const { return Vector3F(x, y, z); }
        [[nodiscard]] constexpr explicit operator Vector2F() const { return Vector2F(x, y); }
        [[nodiscard]] constexpr explicit operator Vector4F() const { return Vector4F(x, y, z, w); }
    };
}
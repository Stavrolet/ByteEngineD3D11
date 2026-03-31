#pragma once

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"

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

        BYTEENGINE_API [[nodiscard]] Vector3f GetEuler();
        BYTEENGINE_API [[nodiscard]] Vector3f GetEuler() const;
        BYTEENGINE_API [[nodiscard]] Vector3f GetEulerInDegrees() const;

        // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::get_axis
        BYTEENGINE_API [[nodiscard]] Vector3f GetAxis() const;
        BYTEENGINE_API [[nodiscard]] RadianF GetAngle() const;
        BYTEENGINE_API void GetAxisAngle(Vector3f& axis, RadianF& angle) const;

        [[nodiscard]] constexpr static float Dot(Quaternion a, Quaternion b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        BYTEENGINE_API [[nodiscard]] static RadianF AngleBetween(Quaternion a, Quaternion b);

        BYTEENGINE_API [[nodiscard]] static Quaternion FromAngleAxis(RadianF angle, Vector3f axis);
        BYTEENGINE_API [[nodiscard]] static Quaternion FromAngleAxis(DegreeF angle, Vector3f axis);

        [[nodiscard]] static constexpr Quaternion FromEulerInRadians(Vector3f eulerAngles)
        {
            float halfPitch = eulerAngles.x * 0.5f;
            float halfYaw = eulerAngles.y * 0.5f;
            float halfRoll = eulerAngles.z * 0.5f;

            float pitchCos = Math::Cos(static_cast<RadianF>(halfPitch));
            float pitchSin = Math::Sin(static_cast<RadianF>(halfPitch));
            float yawCos = Math::Cos(static_cast<RadianF>(halfYaw));
            float yawSin = Math::Sin(static_cast<RadianF>(halfYaw));
            float rollCos = Math::Cos(static_cast<RadianF>(halfRoll));
            float rollSin = Math::Sin(static_cast<RadianF>(halfRoll));

            return Quaternion(
                rollCos * pitchSin * yawCos + rollSin * pitchCos * yawSin,
                rollCos * pitchCos * yawSin - rollSin * pitchSin * yawCos,
                rollSin * pitchCos * yawCos - rollCos * pitchSin * yawSin,
                rollCos * pitchCos * yawCos + rollSin * pitchSin * yawSin
            );
        }

        [[nodiscard]] static constexpr Quaternion FromEuler(Vector3f eulerAngles) { return FromEulerInRadians(eulerAngles * (Math::PI / 180.0f)); }

        [[nodiscard]] static constexpr Quaternion FromEulerInRadians(RadianF pitch, RadianF yaw, RadianF roll) { return FromEulerInRadians(Vector3f(pitch.value, yaw.value, roll.value)); }
        [[nodiscard]] static constexpr Quaternion FromEuler(DegreeF pitch, DegreeF yaw, DegreeF roll) { return FromEulerInRadians(Vector3f(pitch.value, yaw.value, roll.value) * (Math::PI / 180.0f)); }

        BYTEENGINE_API [[nodiscard]] static Quaternion FromLookDirection(Vector3f direction, Vector3f worldUp = Vector3f::Up());
        BYTEENGINE_API [[nodiscard]] static Quaternion FromToRotation(Vector3f from, Vector3f target);

        BYTEENGINE_API [[nodiscard]] static Quaternion SlerpUnclamped(Quaternion from, Quaternion to, float t);
        BYTEENGINE_API [[nodiscard]] static Quaternion Slerp(Quaternion from, Quaternion to, float t);

        [[nodiscard]] constexpr Quaternion operator+() const { return Quaternion(+x, +y, +z, +w); }
        [[nodiscard]] constexpr Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

        constexpr Quaternion operator+(Quaternion q) const { return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w); }
        constexpr Quaternion operator-(Quaternion q) const { return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w); }

        constexpr Quaternion operator+=(Quaternion q)
        {
            *this = *this + q;
            return *this;
        }

        constexpr Quaternion operator-=(Quaternion q)
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

        constexpr Quaternion operator*=(Quaternion q)
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

        [[nodiscard]] constexpr explicit operator Vector3f() const { return Vector3f(x, y, z); }
        [[nodiscard]] constexpr explicit operator Vector2() const { return Vector2(x, y); }
    };
}
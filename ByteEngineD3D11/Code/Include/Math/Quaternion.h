#pragma once

#include "Math/Vector3.h"

namespace ByteEngine::Math
{
    struct Vector2;

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

        constexpr Quaternion(float xyzw = 0.0f)
            : x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr Quaternion(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        { }

        constexpr Quaternion(const float arr[4])
            : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
        { }

        [[nodiscard]] float Length() const;
        [[nodiscard]] constexpr float LengthSquared() const { return x * x + y * y + z * z + w * w; }

        void Normalize();
        [[nodiscard]] Quaternion Normalized() const;
        [[nodiscard]] constexpr bool IsNormalized() const { return Math::IsEqualApproximetly(LengthSquared(), 1.0f, Math::UnitSizeEpsilon); }

        constexpr void Inverse() 
        {
            assert(IsNormalized());
            x = -x;
            y = -y;
            z = -z;
        }

        [[nodiscard]] constexpr Quaternion Inversed() const
        {
            Quaternion copy = *this;
            copy.Inverse();
            return copy;
        }

        [[nodiscard]] Vector3 GetEuler();
        [[nodiscard]] Vector3 GetEuler() const;
        [[nodiscard]] Vector3 GetEulerInDegrees() const;

        [[nodiscard]] Vector3 GetAxis() const;
        [[nodiscard]] RadianF GetAngle() const;
        void GetAxisAngle(Vector3& axis, RadianF& angle) const;

        [[nodiscard]] constexpr static float Dot(Quaternion a, Quaternion b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        [[nodiscard]] static RadianF AngleBetween(Quaternion a, Quaternion b);

        [[nodiscard]] static Quaternion FromAngleAxis(RadianF angle, Vector3 axis);
        [[nodiscard]] static Quaternion FromAngleAxis(DegreeF angle, Vector3 axis);

        [[nodiscard]] static constexpr Quaternion FromEulerInRadians(Vector3 eulerAngles)
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

        [[nodiscard]] static constexpr Quaternion FromEuler(Vector3 eulerAngles) { return FromEulerInRadians(eulerAngles * (Math::PI / 180.0f)); }

        [[nodiscard]] static constexpr Quaternion FromEulerInRadians(RadianF pitch, RadianF yaw, RadianF roll) { return FromEulerInRadians(Vector3(pitch, yaw, roll)); }
        [[nodiscard]] static constexpr Quaternion FromEuler(DegreeF pitch, DegreeF yaw, DegreeF roll) { return FromEulerInRadians(Vector3(pitch, yaw, roll) * (Math::PI / 180.0f)); }

        [[nodiscard]] static Quaternion FromLookDirection(Vector3 direction, Vector3 worldUp = Vector3::Up);
        [[nodiscard]] static Quaternion FromToRotation(Vector3 from, Vector3 target);

        [[nodiscard]] static Quaternion SlerpUnclamped(Quaternion from, Quaternion to, float t);
        [[nodiscard]] static Quaternion Slerp(Quaternion from, Quaternion to, float t);

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

        [[nodiscard]] constexpr explicit operator Vector3() const { return Vector3(x, y, z); }
        [[nodiscard]] explicit operator Vector2() const;
    };
}
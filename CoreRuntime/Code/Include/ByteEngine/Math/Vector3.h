#pragma once

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<AnyNumber T>
    struct Vector2t;

    template<AnyNumber T>
    struct Vector3t
    {
        using RadianT = std::conditional_t<sizeof(T) == 8, RadianD, RadianF>;
        using FloatT = std::conditional_t<sizeof(T) == 8, double, float>;

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            struct
            {
                T r;
                T g;
                T b;
            };

            T data[3];
        };

        explicit constexpr Vector3t(T xyz = 0)
            : x(xyz), y(xyz), z(xyz)
        { }

        constexpr Vector3t(T x, T y, T z)
            : x(x), y(y), z(z)
        { }

        FloatT Length() const { return Math::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z; }

        void Normalize()
        {
            FloatT length = LengthSquared();

            if (length != 0)
                *this /= Math::Sqrt(length);
        }

        Vector3t Normalized() const
        {
            Vector3t copy = *this;
            copy.Normalize();
            return copy;
        }

        constexpr bool IsNormalized() const { return Math::IsEqualApproximetly(1, LengthSquared(), Math::UnitSizeEpsilon); }

        void LimitLength(FloatT maxLength = 1)
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
            {
                currentLength = Math::Sqrt(currentLength);
                *this *= maxLength / currentLength;
            }
        }

        static RadianF AngleBetween(Vector3t from, Vector3t to, Vector3t rotationAxis)
        {
            Vector3t cross = Cross(from, to);
            FloatT unsignedAngle = Math::Atan2(cross.Length(), Dot(from, to));
            FloatT sign = Math::Sign(Dot(cross, rotationAxis));
            return unsignedAngle * sign;
        }

        static RadianF UnsigedAngleBetween(Vector3t from, Vector3t to) { return Math::Atan2(Cross(from, to).Length(), Dot(from, to)); }

        static FloatT Distcance(Vector3t a, Vector3t b) { return Math::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector3t a, Vector3t b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector3t Direction(Vector3t from, Vector3t to)
        {
            Vector3t dir = to - from;
            dir.Normalize();
            return dir;
        }

        static constexpr Vector3t Cross(Vector3t a, Vector3t b)
        {
            return Vector3t(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static constexpr FloatT Dot(Vector3t a, Vector3t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        static constexpr Vector3t Lerp(Vector3t from, Vector3t to, FloatT t) { return from + (to - from) * t; }
        static constexpr Vector3t LerpClamped(Vector3t from, Vector3t to, FloatT t) { return from + (to - from) * Math::Clamp(t); }

        static Vector3t MoveTowards(Vector3t current, Vector3t target, FloatT maxDelta)
        {
            Vector3t vd = target - current;
            FloatT length = vd.Length();

            if (length <= maxDelta || length < Math::Epsilon)
                return target;
            else
                return current + vd / length * maxDelta;
        }

        static constexpr Vector3t Project(Vector3t vec, Vector3t projectOnto) { return projectOnto * (Dot(vec, projectOnto) / projectOnto.LengthSquared()); }
        static constexpr Vector3t ProjectNormalized(Vector3t vec, Vector3t projectOnto) { return projectOnto * Dot(vec, projectOnto); }

        static constexpr Vector3t Reflect(Vector3t vec, Vector3t normal) { return vec - 2 * Dot(vec, normal) * normal; }

        static constexpr bool IsEqualApproximetly(Vector3t a, Vector3t b) requires FloatingPointNumber<T>
        {
            return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y) && Math::IsEqualApproximetly(a.z, b.z);
        }

        static constexpr Vector3t Zero() { return Vector3t(0); }
        static constexpr Vector3t One() { return Vector3t(1); }
        static constexpr Vector3t Up() { return Vector3t(0, 1, 0); }
        static constexpr Vector3t Down() { return Vector3t(0, -1, 0); }
        static constexpr Vector3t Left() { return Vector3t(-1, 0, 0); }
        static constexpr Vector3t Right() { return Vector3t(1, 0, 0); }
        static constexpr Vector3t Forward() { return Vector3t(0, 0, 1); }
        static constexpr Vector3t Back() { return Vector3t(0, 0, -1); }

        constexpr Vector3t operator+() const { return Vector3t(+x, +y, +z); }
        constexpr Vector3t operator-() const { return Vector3t(-x, -y, -z); }

        constexpr Vector3t operator+(Vector3t a) const { return Vector3t(a.x + x, a.y + y, a.z + z); }
        constexpr Vector3t operator+(T s) const { return Vector3t(s + x, s + y, s + z); }
        constexpr Vector3t operator-(Vector3t a) const { return Vector3t(x - a.x, y - a.y, z - a.z); }
        constexpr Vector3t operator-(T s) const { return Vector3t(x - s, y - s, z - s); }

        constexpr Vector3t& operator+=(Vector3t a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr Vector3t& operator+=(T s)
        {
            x += s;
            y += s;
            z += s;
            return *this;
        }

        constexpr Vector3t& operator-=(Vector3t a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr Vector3t& operator-=(T a)
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        constexpr Vector3t operator*(Vector3t a) const { return Vector3t(x * a.x, y * a.y, z * a.z); }
        constexpr Vector3t operator*(T s) const { return Vector3t(x * s, y * s, z * s); }
        friend constexpr Vector3t operator*(T s, Vector3t v) { return v * s; }

        constexpr Vector3t& operator*=(Vector3t a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr Vector3t& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        constexpr Vector3t operator/(Vector3t a) const { return Vector3t(x / a.x, y / a.y, z / a.z); }
        constexpr Vector3t operator/(T s) const { return Vector3t(x / s, y / s, z / s); }

        constexpr Vector3t& operator/=(Vector3t a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr Vector3t& operator/=(T s)
        {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        constexpr bool operator==(Vector3t other) const { return x == other.x && y == other.y && z == other.z; }
        constexpr bool operator!=(Vector3t other) const { return !(*this == other); }

        T& operator[](int32 index)
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        T operator[](int32 index) const
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }
        
        template<AnyNumber U>
            requires !std::is_same_v<T, U>&& std::is_convertible_v<T, U>
        operator Vector3t<U>() const { return Vector3t<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }

        operator Vector2t<T>() const;

        template<AnyNumber U>
            requires !std::is_same_v<T, U>&& std::is_convertible_v<T, U>
        operator Vector2t<U>() const;
    };

    using Vector3f = Vector3t<float>;
    using Vector3d = Vector3t<double>;
    using Vector3i = Vector3t<int32>;
    using Vector3 = Vector3f;

}
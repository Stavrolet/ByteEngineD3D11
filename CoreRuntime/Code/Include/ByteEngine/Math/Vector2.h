#pragma once

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Math/Concepts.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<AnyNumber T>
    struct Vector3t;

    template<AnyNumber T>
    struct Vector4t;

    template<AnyNumber T>
    struct Vector2t
    {
        using RadianT = std::conditional_t<sizeof(T) == sizeof(double), RadianD, RadianF>;
        using FloatT = std::conditional_t<sizeof(T) == 8, double, float>;

        union
        {
            struct
            {
                T x;
                T y;
            };

            struct
            {
                T u;
                T v;
            };

            struct
            {
                T width;
                T height;
            };

            T data[2];
        };

        explicit constexpr Vector2t(T xy = 0)
            : x(xy), y(xy)
        { }

        constexpr Vector2t(T x, T y)
            : x(x), y(y)
        { }

        FloatT Length() const { return Math::Sqrt(static_cast<FloatT>(x * x + y * y)); }
        constexpr FloatT LengthSquared() const { return x * x + y * y; }

        void Normalize() requires FloatingPointNumber<T>
        {
            FloatT lengthSq = LengthSquared();

            if (lengthSq > Math::Epsilon)
                *this /= Math::Sqrt(lengthSq);
            else
                *this = Zero();
        }

        Vector2t Normalized() const requires FloatingPointNumber<T>
        {
            Vector2t copy = *this;
            copy.Normalize();
            return copy;
        }

        bool IsNormalized() const requires FloatingPointNumber<T>
        {
            return Math::IsEqualApproximetly(static_cast<FloatT>(1), LengthSquared(), static_cast<FloatT>(Math::UnitSizeEpsilon));
        }

        // RotateBy implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::rotated
        constexpr void RotateBy(RadianT angle) requires FloatingPointNumber<T>
        {
            FloatT sin, cos;

            if constexpr (std::is_same_v<FloatT, float>)
            {
                Math::SinCos(sin, cos, angle);
            }
            else
            {
                sin = Math::Sin(angle);
                cos = Math::Cos(angle);
            }

            T oldX = x;
            x = oldX * cos - y * sin;
            y = oldX * sin + y * cos;
        }

        constexpr Vector2t RotatedBy(RadianT angle) const requires FloatingPointNumber<T>
        {
            Vector2t copy = *this;
            copy.RotateBy(angle);
            return copy;
        }

        void LimitLength(T maxLength = 1) requires FloatingPointNumber<T>
        {
            T currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
            {
                currentLength = Math::Sqrt(currentLength);
                *this *= maxLength / currentLength;
            }
        }

        static RadianT AngleBetween(Vector2t from, Vector2t to) requires FloatingPointNumber<T>
        {
            FloatT cross = Cross(from, to);
            FloatT dot = Dot(from, to);
            return Math::Atan2(cross, dot);
        }

        static RadianT UnsigedAngleBetween(Vector2t from, Vector2t to) requires FloatingPointNumber<T>
        {
            return RadianT(Math::Abs(AngleBetween(from, to).value));
        }

        static T Distcance(Vector2t a, Vector2t b) { return Math::Sqrt(static_cast<FloatT>((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y))); }
        static constexpr T DistcanceSquared(Vector2t a, Vector2t b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }

        static Vector2t Direction(Vector2t from, Vector2t to)
        {
            Vector2t dir = to - from;

            if constexpr (FloatingPointNumber<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr T Cross(Vector2t a, Vector2t b) requires FloatingPointNumber<T>
        {
            return a.x * b.y - a.y * b.x;
        }

        static constexpr T Dot(Vector2t a, Vector2t b) requires FloatingPointNumber<T>
        {
            return a.x * b.x + a.y * b.y;
        }

        static constexpr Vector2t FromAngle(RadianT angle) requires FloatingPointNumber<T>
        {
            Vector2t<FloatT> vec;
            Math::SinCos(vec.x, vec.y, angle);
            return vec;
        }

        static constexpr Vector2t FromAngle(RadianT angle, T length) requires FloatingPointNumber<T>
        {
            return FromAngle(angle) * length;
        }

        static constexpr Vector2t Lerp(Vector2t from, Vector2t to, FloatT t) requires FloatingPointNumber<T>
        {
            assert(t >= 0 && t <= 1);
            return from + (to - from) * t;
        }

        static constexpr Vector2t LerpClamped(Vector2t from, Vector2t to, FloatT t) requires FloatingPointNumber<T>
        {
            return from + (to - from) * Math::Clamp(t);
        }

        // Slerp implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::slerp
        static Vector2t Slerp(Vector2t from, Vector2t to, FloatT t) requires FloatingPointNumber<T>
        {
            assert(t >= 0 && t <= 1);

            FloatT startLength = from.LengthSquared();
            FloatT endLength = to.LengthSquared();

            if (startLength == 0 || endLength == 0)
                return Lerp(from, to, t);

            startLength = Math::Sqrt(startLength);
            FloatT resultLength = Math::Lerp(startLength, Math::Sqrt(endLength), t);
            RadianT angle = AngleBetween(from, to);

            from.RotateBy(angle * t);
            return from * (resultLength / startLength);
        }

        // MoveTowards implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::move_toward
        static Vector2t MoveTowards(Vector2t current, Vector2t target, FloatT maxDelta) requires FloatingPointNumber<T>
        {
            Vector2t direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Math::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static constexpr Vector2t Project(Vector2t vec, Vector2t projectOnto) requires FloatingPointNumber<T>
        {
            T dot = Dot(vec, projectOnto);
            if (dot < Math::Epsilon)
                return Zero();

            return projectOnto * (dot / projectOnto.LengthSquared());
        }

        static Vector2t ProjectNormalized(Vector2t vec, Vector2t projectOnto) requires FloatingPointNumber<T>
        {
            assert(projectOnto.IsNormalized() || IsEqualApproximetly(projectOnto, Zero()));
            return projectOnto * Dot(vec, projectOnto);
        }

        static Vector2t Reflect(Vector2t vec, Vector2t normal) requires FloatingPointNumber<T>
        {
            assert(normal.IsNormalized() || IsEqualApproximetly(normal, Zero()));
            return vec - T(2) * Dot(vec, normal) * normal;
        }

        static bool IsEqualApproximetly(Vector2t a, Vector2t b) requires FloatingPointNumber<T> { return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y); }

        static constexpr Vector2t Min(Vector2t a, Vector2t b) { return Vector2t(Math::Min(a.x, b.x), Math::Min(a.y, b.y)); }
        static constexpr Vector2t Min(Vector2t a, Vector2t b, Vector2t c) { return Vector2t(Math::Min(a.x, b.x, c.x), Math::Min(a.y, b.y, c.y)); }

        static constexpr Vector2t Max(Vector2t a, Vector2t b) { return Vector2t(Math::Max(a.x, b.x), Math::Max(a.y, b.y)); }
        static constexpr Vector2t Max(Vector2t a, Vector2t b, Vector2t c) { return Vector2t(Math::Max(a.x, b.x, c.x), Math::Max(a.y, b.y, c.y)); }

        static constexpr Vector2t Zero() { return Vector2t(0); }
        static constexpr Vector2t One() { return Vector2t(1); }
        static constexpr Vector2t Up() { return Vector2t(0, 1); }
        static constexpr Vector2t Down() { return Vector2t(0, -1); }
        static constexpr Vector2t Left() { return Vector2t(-1, 0); }
        static constexpr Vector2t Right() { return Vector2t(1, 0); }

        constexpr Vector2t operator+() const { return Vector2t(+x, +y); }
        constexpr Vector2t operator-() const { return Vector2t(-x, -y); }

        constexpr Vector2t operator+(Vector2t a) const { return Vector2t(a.x + x, a.y + y); }
        constexpr Vector2t operator+(T a) const { return Vector2t(a + x, a + y); }
        constexpr Vector2t operator-(Vector2t a) const { return Vector2t(x - a.x, y - a.y); }
        constexpr Vector2t operator-(T a) const { return Vector2t(x - a, y - a); }

        constexpr Vector2t& operator+=(Vector2t a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr Vector2t& operator+=(T a)
        {
            x += a;
            y += a;
            return *this;
        }

        constexpr Vector2t& operator-=(Vector2t a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr Vector2t& operator-=(T a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        constexpr Vector2t operator*(Vector2t a) const { return Vector2t(x * a.x, y * a.y); }
        constexpr Vector2t operator*(T s) const { return Vector2t(x * s, y * s); }
        friend constexpr Vector2t operator*(T s, Vector2t v) { return v * s; }

        constexpr Vector2t& operator*=(Vector2t a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr Vector2t& operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vector2t operator/(Vector2t a) const { return Vector2t(x / a.x, y / a.y); }
        constexpr Vector2t operator/(T s) const { return Vector2t(x / s, y / s); }

        constexpr Vector2t& operator/=(Vector2t a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr Vector2t& operator/=(T s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        constexpr bool operator==(Vector2t other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(Vector2t other) const { return !(*this == other); }

        T& operator[](int32 index)
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        T operator[](int32 index) const
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector2t<U>() const { return Vector2t<U>(static_cast<U>(x), static_cast<U>(y)); }

        operator Vector3t<T>() const;

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector3t<U>() const;

        operator Vector4t<T>() const;

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector4t<U>() const;
    };

    using Vector2f = Vector2t<float>;
    using Vector2d = Vector2t<double>;
    using Vector2i = Vector2t<int32>;
    using Vector2 = Vector2f;
}
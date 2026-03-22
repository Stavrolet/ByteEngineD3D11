#pragma once

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Math/Concepts.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<AnyNumber T>
    struct Vector3t;

    template<AnyNumber T>
    struct Vector2t
    {
        using RadianT = std::conditional_t<sizeof(T) == 8, RadianD, RadianF>;
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

        FloatT Length() const { return Math::Sqrt(x * x + y * y); }
        constexpr FloatT LengthSquared() const { return x * x + y * y; }

        void Normalize()
        {
            T length = LengthSquared();

            if (length != 0)
            {
                length = Math::Sqrt(length);
                x /= length;
                y /= length;
            }
        }

        Vector2t Normalized() const
        {
            Vector2t copy = *this;
            copy.Normalize();
            return copy;
        }

        constexpr bool IsNormalized() const { return Math::IsEqualApproximetly(1, LengthSquared(), Math::UnitSizeEpsilon); }

        constexpr void RotateBy(RadianT angle)
        {
            FloatT sin, cos;
            Math::SinCos(sin, cos, angle);
            T oldX = x;
            x = oldX * cos - y * sin;
            y = oldX * sin + y * cos;
        }

        constexpr Vector2t RotatedBy(RadianT angle) const
        {
            Vector2t copy = *this;
            copy.RotateBy(angle);
            return copy;
        }

        void LimitLength(T maxLength = 1)
        {
            T currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
            {
                currentLength = Math::Sqrt(currentLength);
                *this *= maxLength / currentLength;
            }
        }

        static RadianT AngleBetween(Vector2t from, Vector2t to)
        {
            FloatT cross = Cross(from, to);
            FloatT dot = Dot(from, to);
            return Math::Atan2(cross, dot);
        }

        static RadianT UnsigedAngleBetween(Vector2t from, Vector2t to) { return Math::Abs(AngleBetween(from, to)); }

        static T Distcance(Vector2t a, Vector2t b) { return Math::Sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }
        static constexpr T DistcanceSquared(Vector2t a, Vector2t b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }

        static Vector2t Direction(Vector2t from, Vector2t to)
        {
            Vector2t dir = to - from;
            dir.Normalize();
            return dir;        
        }

        static RadianT DirectionAngle(Vector2t from, Vector2t to)
        {
            Vector2t dir = to - from;
            dir.Normalize();
            return Math::Atan2(static_cast<FloatT>(dir.x), dir.y);
        }

        static constexpr T Cross(Vector2t a, Vector2t b) { return a.x * b.y - a.y * b.x; }
        static constexpr T Dot(Vector2t a, Vector2t b) { return a.x * b.x + a.y * b.y; }

        static constexpr Vector2t FromAngle(RadianT angle)
        {
            Vector2t<FloatT> vec;
            Math::SinCos(vec.x, vec.y, angle);
            return vec;
        }

        static constexpr Vector2t FromAngle(RadianT angle, T length) { return FromAngle(angle) * length; }

        static constexpr Vector2t Lerp(Vector2t from, Vector2t to, FloatT t) { return from + (to - from) * t; }
        static constexpr Vector2t LerpClamped(Vector2t from, Vector2t to, FloatT t) { return from + (to - from) * Math::Clamp(t); }
        static Vector2t Slerp(Vector2t from, Vector2t to, FloatT t)
        {
            assert(t >= 0 && t <= 1);

            T startLength = from.LengthSquared();
            T endLength = to.LengthSquared();

            if (startLength == 0 || endLength == 0)
                return Lerp(from, to, t);

            startLength = Math::Sqrt(startLength);
            T resultLength = Math::Lerp(startLength, Math::Sqrt(endLength), t);
            T angle = AngleBetween(from, to);

            from.RotateBy(angle * t);
            return from * (resultLength / startLength);
        }

        static Vector2t MoveTowards(Vector2t current, Vector2t target, FloatT maxDelta)
        {
            Vector2t vd = target - current;
            T length = vd.Length();

            if (length <= maxDelta || length < Math::Epsilon)
                return target;
            else
                return current + vd / length * maxDelta;
        }

        static constexpr Vector2t Project(Vector2t vec, Vector2t projectOnto) { return projectOnto * (Dot(vec, projectOnto) / projectOnto.LengthSquared()); }
        static constexpr Vector2t ProjectNormalized(Vector2t vec, Vector2t projectOnto) { return projectOnto * Dot(vec, projectOnto); }

        static constexpr Vector2t Reflect(Vector2t vec, Vector2t normal) { return vec - 2.0f * Dot(vec, normal) * normal; }

        static constexpr bool IsEqualApproximetly(Vector2t a, Vector2t b) requires FloatingPointNumber<T> { return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y); }

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
            requires !std::is_same_v<T, U> && std::is_convertible_v<T, U>
        operator Vector2t<U>() const { return Vector2t<U>(static_cast<U>(x), static_cast<U>(y)); }

        operator Vector3t<T>() const;

        template<AnyNumber U>
            requires !std::is_same_v<T, U>&& std::is_convertible_v<T, U>
        operator Vector3t<U>() const;
    };

    using Vector2f = Vector2t<float>;
    using Vector2d = Vector2t<double>;
    using Vector2i = Vector2t<int32>;
    using Vector2 = Vector2f;
}
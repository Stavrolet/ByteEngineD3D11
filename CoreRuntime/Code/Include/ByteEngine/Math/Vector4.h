#pragma once

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<AnyNumber T>
    struct Vector2t;

    template<AnyNumber T>
    struct Vector3t;

    template<AnyNumber T>
    struct Vector4t
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
                T w;
            };

            struct
            {
                T r;
                T g;
                T b;
                T a;
            };

            T data[4];
        };

        explicit constexpr Vector4t(T xyzw = 0)
            : x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr Vector4t(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        { }

        FloatT Length() const { return Math::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z + w * w; }

        void Normalize() requires FloatingPointNumber<T>
        {
            FloatT length = LengthSquared();

            if (length > Math::Epsilon)
                *this /= Math::Sqrt(length);
            else
                *this = Zero();
        }

        Vector4t Normalized() const requires FloatingPointNumber<T>
        {
            Vector4t copy = *this;
            copy.Normalize();
            return copy;
        }

        constexpr bool IsNormalized() const requires FloatingPointNumber<T>
        {
            return Math::IsEqualApproximetly(static_cast<FloatT>(1), LengthSquared(), static_cast<FloatT>(Math::UnitSizeEpsilon));
        }

        void LimitLength(FloatT maxLength = 1) requires FloatingPointNumber<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
                *this *= maxLength / Math::Sqrt(currentLength);
        }

        static FloatT Distcance(Vector4t a, Vector4t b) { return Math::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector4t a, Vector4t b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector4t Direction(Vector4t from, Vector4t to)
        {
            Vector4t dir = to - from;

            if constexpr (FloatingPointNumber<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr FloatT Dot(Vector4t a, Vector4t b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        static constexpr Vector4t Lerp(Vector4t from, Vector4t to, FloatT t) requires FloatingPointNumber<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector4t LerpClamped(Vector4t from, Vector4t to, FloatT t) requires FloatingPointNumber<T>
        {
            return from + (to - from) * Math::Clamp(t);
        }

        static Vector4t MoveTowards(Vector4t current, Vector4t target, FloatT maxDelta) requires FloatingPointNumber<T>
        {
            Vector4t direction = target - current;
            FloatT distance = vd.Length();

            if (distance <= maxDelta || distance < Math::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static constexpr bool IsEqualApproximetly(Vector4t a, Vector4t b) requires FloatingPointNumber<T>
        {
            return Math::IsEqualApproximetly(a.x, b.x) && Math::IsEqualApproximetly(a.y, b.y) && Math::IsEqualApproximetly(a.z, b.z);
        }

        static constexpr Vector4t Zero() { return Vector4t(0); }
        static constexpr Vector4t One() { return Vector4t(1); }

        constexpr Vector4t operator+() const { return Vector4t(+x, +y, +z, +w); }
        constexpr Vector4t operator-() const { return Vector4t(-x, -y, -z, -w); }

        constexpr Vector4t operator+(Vector4t a) const { return Vector4t(a.x + x, a.y + y, a.z + z, a.w + w); }
        constexpr Vector4t operator+(T s) const { return Vector4t(s + x, s + y, s + z, s + w); }
        constexpr Vector4t operator-(Vector4t a) const { return Vector4t(x - a.x, y - a.y, z - a.z, w - a.w); }
        constexpr Vector4t operator-(T s) const { return Vector4t(x - s, y - s, z - s, w - s); }

        constexpr Vector4t& operator+=(Vector4t a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }

        constexpr Vector4t& operator+=(T s)
        {
            x += s;
            y += s;
            z += s;
            w += s;
            return *this;
        }

        constexpr Vector4t& operator-=(Vector4t a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }

        constexpr Vector4t& operator-=(T a)
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }

        constexpr Vector4t operator*(Vector4t a) const { return Vector4t(x * a.x, y * a.y, z * a.z, w * a.w); }
        constexpr Vector4t operator*(T s) const { return Vector4t(x * s, y * s, z * s, w * s); }
        friend constexpr Vector4t operator*(T s, Vector4t v) { return v * s; }

        constexpr Vector4t& operator*=(Vector4t a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }

        constexpr Vector4t& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        constexpr Vector4t operator/(Vector4t a) const { return Vector4t(x / a.x, y / a.y, z / a.z, w / a.w); }
        constexpr Vector4t operator/(T s) const { return Vector4t(x / s, y / s, z / s, w / s); }

        constexpr Vector4t& operator/=(Vector4t a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }

        constexpr Vector4t& operator/=(T s)
        {
            x /= s;
            y /= s;
            z /= s;
            w /= s;
            return *this;
        }

        constexpr bool operator==(Vector4t other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(Vector4t other) const { return !(*this == other); }

        T& operator[](int32 index)
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        T operator[](int32 index) const
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector4t<U>() const { return Vector4t<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }

        operator Vector2t<T>() const;

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector2t<U>() const;

        operator Vector3t<T>() const;

        template<AnyNumber U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector3t<U>() const;
    };

    using Vector4f = Vector4t<float>;
    using Vector4d = Vector4t<double>;
    using Vector4i = Vector4t<int32>;
    using Vector4 = Vector4f;
}
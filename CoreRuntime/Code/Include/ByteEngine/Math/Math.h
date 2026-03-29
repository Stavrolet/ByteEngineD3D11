#pragma once

#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <limits>
#include <ranges>
#include <vector>

#undef min
#undef max

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Primitives.h"
#include "ByteEngine/Math/Concepts.h"

namespace ByteEngine::Math
{
    // not type aliases for type safety

    template<FloatingPointNumber T>
    struct RadianT
    {
        T value;

        explicit constexpr RadianT(T value = 0)
            : value(value)
        { }
         
        constexpr RadianT operator+() const { return RadianT(+value); }
        constexpr RadianT operator-() const { return RadianT(-value); }

        constexpr RadianT operator+(RadianT other) const { return RadianT(value + other.value); }
        constexpr RadianT operator+(T other) const { return RadianT(value + other); }
        constexpr RadianT operator-(RadianT other) const { return RadianT(value - other.value); }
        constexpr RadianT operator-(T other) const { return RadianT(value - other); }

        constexpr RadianT operator*(RadianT other) const { return RadianT(value * other.value); }
        constexpr RadianT operator*(T other) const { return RadianT(value * other); }
        friend constexpr RadianT operator*(T other, RadianT rad) { return RadianT(rad.value * other); }

        constexpr RadianT operator/(RadianT other) const { return RadianT(value / other.value); }
        constexpr RadianT operator/(T other) const { return RadianT(value / other); }
        friend consteval RadianT operator/(T other, RadianT rad) { return RadianT(other / rad.value); }

        constexpr RadianT& operator+=(RadianT other) 
        {
            value += other.value;
            return *this;
        }

        constexpr RadianT& operator+=(T other)
        {
            value += other;
            return *this;
        }

        constexpr RadianT& operator-=(RadianT other)
        {
            value -= other.value;
            return *this;
        }

        constexpr RadianT& operator-=(T other)
        {
            value -= other;
            return *this;
        }

        constexpr RadianT& operator*=(RadianT other)
        {
            value *= other.value;
            return *this;
        }

        constexpr RadianT& operator*=(T other)
        {
            value *= other;
            return *this;
        }

        constexpr RadianT& operator/=(RadianT other)
        {
            value /= other.value;
            return *this;
        }

        constexpr RadianT& operator/=(T other)
        {
            value /= other;
            return *this;
        }

        constexpr auto operator<=>(const RadianT&) const = default;
    };

    template<FloatingPointNumber T>
    struct DegreeT
    {
        T value;

        constexpr DegreeT(T value = 0)
            : value(value)
        { }

        constexpr DegreeT operator+() const { return DegreeT(+value); }
        constexpr DegreeT operator-() const { return DegreeT(-value); }

        constexpr DegreeT operator+(DegreeT other) const { return DegreeT(value + other.value); }
        constexpr DegreeT operator+(T other) const { return DegreeT(value + other); }
        constexpr DegreeT operator-(DegreeT other) const { return DegreeT(value - other.value); }
        constexpr DegreeT operator-(T other) const { return DegreeT(value - other); }

        constexpr DegreeT operator*(DegreeT other) const { return DegreeT(value * other.value); }
        constexpr DegreeT operator*(T other) const { return DegreeT(value * other); }
        friend constexpr DegreeT operator*(T other, DegreeT deg) { return DegreeT(deg.value * other); }

        constexpr DegreeT operator/(DegreeT other) const { return DegreeT(value / other.value); }
        constexpr DegreeT operator/(T other) const { return DegreeT(value / other); }
        friend consteval DegreeT operator/(T other, DegreeT deg) { return DegreeT(other / deg.value); }

        constexpr DegreeT& operator+=(DegreeT other)
        {
            value += other.value;
            return *this;
        }

        constexpr DegreeT& operator+=(T other)
        {
            value += other;
            return *this;
        }

        constexpr DegreeT& operator-=(DegreeT other)
        {
            value -= other.value;
            return *this;
        }

        constexpr DegreeT& operator-=(T other)
        {
            value -= other;
            return *this;
        }

        constexpr DegreeT& operator*=(DegreeT other)
        {
            value *= other.value;
            return *this;
        }

        constexpr DegreeT& operator*=(T other)
        {
            value *= other;
            return *this;
        }

        constexpr DegreeT& operator/=(DegreeT other)
        {
            value /= other.value;
            return *this;
        }

        constexpr DegreeT& operator/=(T other)
        {
            value /= other;
            return *this;
        }

        constexpr auto operator<=>(const DegreeT&) const = default;
    };

    using RadianF = RadianT<float>;
    using RadianD = RadianT<double>;

    using DegreeF = DegreeT<float>;
    using DegreeD = DegreeT<double>;

    constexpr RadianF operator""_rf(long double value) { return RadianF(value); }
    constexpr RadianD operator""_rd(long double value) { return RadianD(value); }
    constexpr DegreeF operator""_df(long double value) { return DegreeF(value); }
    constexpr DegreeD operator""_dd(long double value) { return DegreeD(value); }
}

namespace ByteEngine::Math::Math
{
    namespace Internal
    {
        template<typename... Args>
        concept AnyFloating = (FloatingPointNumber<Args> || ...);
    }

    constexpr float Infinity = std::numeric_limits<float>::infinity();
    constexpr double InfinityD = std::numeric_limits<double>::infinity();
    constexpr float NegativeInfinity = -std::numeric_limits<float>::infinity();
    constexpr double NegativeInfinityD = -std::numeric_limits<double>::infinity();

    constexpr float PI = 3.141592654f;
    constexpr double PI_D = 3.141592653589793;

    constexpr float Epsilon = 1e-5f;
    constexpr double EpsilonD = 1e-8f;
    constexpr RadianF AngleEpsilon = RadianF(1e-4f);
    constexpr float UnitSizeEpsilon = 1e-4f;

    constexpr RadianF DegToRad(DegreeF deg) noexcept { return deg.value * (PI / 180.0_rf); }
    constexpr RadianD DegToRad(DegreeD deg) noexcept { return deg.value * (PI_D / 180.0_rd); }

    constexpr DegreeF RadToDeg(RadianF rad) noexcept { return rad.value * (180.0_df / PI); }
    constexpr DegreeD RadToDeg(RadianD rad) noexcept { return rad.value * (180.0_dd / PI_D); }

    // Sin implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    [[nodiscard]] constexpr float Sin(RadianF rad) noexcept
    {
        if (rad == 0.0_rf)
        {
            return 0.0f;
        }

        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        if (y > PI / 2.0f)
        {
            y = PI - y;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
        }

        // 11-degree minimax approximation
        float y2 = y * y;
        return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
    }

    BYTEENGINE_API [[nodiscard]] double Sin(RadianD rad);

    // Cos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    [[nodiscard]] constexpr float Cos(RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
        float sign;
        if (y > PI / 2.0f)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        // 10-degree minimax approximation
        float y2 = y * y;
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        return sign * p;
    }

    BYTEENGINE_API [[nodiscard]] double Cos(RadianD rad);

    template<FloatingPointNumber T>
    [[nodiscard]] T Tan(RadianT<T> rad) { return std::tan(rad); }

    // Asin implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    BYTEENGINE_API [[nodiscard]] RadianF Asin(float value) noexcept;

    BYTEENGINE_API [[nodiscard]] RadianD Asin(double value);

    // Acos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    BYTEENGINE_API [[nodiscard]] RadianF Acos(float value) noexcept;

    BYTEENGINE_API [[nodiscard]] RadianD Acos(double value);

    template<FloatingPointNumber T>
    [[nodiscard]] RadianT<T> Atan(T value) { return std::atan(value); }

    // SinCos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    [[nodiscard]] constexpr void SinCos(float& sin, float& cos, RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > PI / 2.0f)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        cos = sign * p;
    }

    template<FloatingPointNumber T, FloatingPointNumber U>
    [[nodiscard]] RadianT<std::common_type_t<T, U>> Atan2(T x, U y) { return RadianT<std::common_type_t<T, U>>(std::atan2(x, y)); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Sqrt(T value) { return std::sqrt(value); }

    template<AnyNumber T>
    [[nodiscard]] inline T Abs(T value) noexcept { return std::abs(value); }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Clamp(T value, U min, V max) noexcept
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;

        return value;
    }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Clamp(T value, T min = 0, T max = 1) noexcept
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;

        return value;
    }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Round(T value) noexcept { return std::round(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Ceil(T value) noexcept { return std::ceil(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Floor(T value) noexcept { return std::floor(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Sign(T value) noexcept { return static_cast<T>((value > 0) - (value < 0)); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Fract(T value) noexcept { return value - Floor(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Exp(T value) noexcept { return std::exp(value); }

    template<AnyNumber T, AnyNumber U>
    [[nodiscard]] inline auto Pow(T value, U power) noexcept { return std::pow(static_cast<std::common_type_t<T, U>>(value), static_cast<std::common_type_t<T, U>>(power)); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log(T value) noexcept { return std::log(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log10(T value) noexcept { return std::log10(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log2(T value) noexcept { return std::log2(value); }

    template<FloatingPointNumber T, FloatingPointNumber U>
    [[nodiscard]] inline auto LogN(T value, U base) noexcept { return Log(value) / Log(base); }

    template<FloatingPointNumber T, FloatingPointNumber U>
    [[nodiscard]] inline auto Fmod(T x, U y) noexcept { return std::fmod(x, y); }

    [[nodiscard]] inline bool IsEqualApproximetly(float right, float left, float tolerance = Epsilon) noexcept { return Abs(left - right) < tolerance; }
    [[nodiscard]] inline bool IsEqualApproximetly(double right, double left, double tolerance = EpsilonD) noexcept { return Abs(left - right) < tolerance; }

    template<FloatingPointNumber T, FloatingPointNumber U, FloatingPointNumber V>
    [[nodiscard]] inline auto LoopValue(T t, U start, V end)
    {
        using Common = std::common_type_t<T, U, V>;

        Common range = end - start;
        Common offset = t - start;

        return Fmod(Fmod(offset, range) + range, range) + start;
    }

    template<IntegerNumber T, IntegerNumber U, IntegerNumber V>
    [[nodiscard]] constexpr auto LoopValue(T t, U start, V end)
    {
        using Common = std::common_type_t<T, U, V>;

        Common range = end - start;
        Common offset = t - start;

        return (offset % range + range) % range + start;
    }

    template<AnyNumber T, AnyNumber U>
        requires Internal::AnyFloating<T, U>
    [[nodiscard]] inline auto PingPong(T t, U length) noexcept
    {
        using Common = std::common_type_t<T, U>;
        return (length != 0.0f) ? Abs(Fract((t - length) / (length * 2.0f)) * length * 2.0f - length) : 0.0f;
    }

    BYTEENGINE_API [[nodiscard]] RadianF AngleDifference(RadianF from, RadianF to) noexcept;
    BYTEENGINE_API [[nodiscard]] RadianD AngleDifference(RadianD from, RadianD to) noexcept;

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto Lerp(T from, U to, V t) noexcept { return from + (to - from) * t; }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto LerpClamped(T from, U to, V t) noexcept { return from + (to - from) * Clamp(t); }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto InverseLerp(T from, U to, V t) noexcept { return (t - from) / (to - from); }

    BYTEENGINE_API [[nodiscard]] RadianF LerpAngle(RadianF from, RadianF to, float t) noexcept;
    BYTEENGINE_API [[nodiscard]] RadianD LerpAngle(RadianD from, RadianD to, double t) noexcept;

    BYTEENGINE_API [[nodiscard]] RadianF LerpAngleClamped(RadianF from, RadianF to, float t) noexcept;
    BYTEENGINE_API [[nodiscard]] RadianD LerpAngleClamped(RadianD from, RadianD to, double t) noexcept;

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] inline auto MoveTowards(T current, U target, V maxDelta) noexcept
    {
        if (Abs(target - current) <= maxDelta)
            return target;

        return current + Sign(target - current) * maxDelta;
    }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] inline auto SmoothStep(T edge0, U edge1, V x) noexcept
    {
        using Common = std::common_type_t<T, U, V>;

        if (IsEqualApproximetly(static_cast<Common>(edge0), static_cast<Common>(edge1)))
            return Common(0);

        if (x <= edge0)
            return Common(0);
        else if (x >= edge1)
            return Common(1);

        Common t = Clamp((x - edge0) / (edge1 - edge0));
        return t * t * (3.0f - 2.0f * t);
    }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsFinite(T value) noexcept { return std::isfinite(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsNaN(T value) noexcept { return std::isnan(value); }

    template<AnyNumber T, AnyNumber U, AnyNumber V, AnyNumber W, AnyNumber X>
    [[nodiscard]] constexpr auto Remap(T value, U oldStart, V oldEnd, W newStart, X newEnd) noexcept
    {
        return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
    }

    template<AnyNumber T = float, std::ranges::input_range R>
        requires AnyNumber<std::ranges::range_value_t<R>>
    [[nodiscard]] constexpr T Average(const R& range)
    {
        std::ranges::range_value_t<R> sum = 0;
        size_t count = 0;

        for (auto el : range)
        {
            sum += el;
            count++;
        }

        if (count == 0)
            return 0;

        return static_cast<T>(sum) / count;
    }

    template<AnyNumber T = float, AnyNumber U>
    [[nodiscard]] constexpr T Average(std::initializer_list<U> list) { return Average<T, std::initializer_list<U>>(list); }

    template<AnyNumber T, AnyNumber U>
    [[nodiscard]] constexpr std::common_type_t<T, U> Min(T a, U b) { return a < b ? a : b; }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Min(T a, U b, V c) { return Min(a, Min(b, c)); }

    template<std::ranges::range R>
    [[nodiscard]] constexpr std::ranges::range_value_t<R> Min(const R& range) { return std::ranges::min(range); }

    template<AnyNumber T, AnyNumber U>
    [[nodiscard]] constexpr std::common_type_t<T, U> Max(T a, U b) { return a > b ? a : b; }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Max(T a, U b, V c) { return Max(a, Max(b, c)); }

    template<std::ranges::range R>
    [[nodiscard]] constexpr std::ranges::range_value_t<R> Max(const R& range) { return std::ranges::max(range); }
}
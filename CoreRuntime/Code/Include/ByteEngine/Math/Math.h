#pragma once

#include <cassert>
#include <cmath>
#include <concepts>
#include <iterator>
#include <limits>

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

        constexpr RadianT(T value = 0.0)
            : value(value)
        { }

        constexpr operator T() const { return value; }
    };

    template<FloatingPointNumber T>
    struct DegreeT
    {
        T value;

        constexpr DegreeT(T value = 0.0)
            : value(value)
        { }

        constexpr operator T() const { return value; }
    };

    using RadianF = RadianT<float>;
    using RadianD = RadianT<double>;

    using DegreeF = DegreeT<float>;
    using DegreeD = DegreeT<double>;
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
    constexpr RadianF AngleEpsilon = 1e-4f;
    constexpr float UnitSizeEpsilon = 1e-4f;

    constexpr RadianF DegToRad(DegreeF deg) noexcept { return deg * (PI / 180.0f); }
    constexpr RadianD DegToRad(DegreeD deg) noexcept { return deg * (PI_D / 180.0); }

    constexpr DegreeF RadToDeg(RadianF rad) noexcept { return rad * (180.0f / PI); }
    constexpr DegreeD RadToDeg(RadianD rad) noexcept { return rad * (180.0 / PI_D); }

    // From DirectXMath.h
    // 
    //-------------------------------------------------------------------------------------
    // DirectXMath.h -- SIMD C++ Math library
    //
    // Copyright (c) Microsoft Corporation.
    // Licensed under the MIT License.
    //
    // http://go.microsoft.com/fwlink/?LinkID=615560
    //-------------------------------------------------------------------------------------
    [[nodiscard]] constexpr float Sin(RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / PI * 2.0f * rad;
        if (rad >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad - PI * 2.0f * quotient;

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

    // From DirectXMath.h
    // 
    //-------------------------------------------------------------------------------------
    // DirectXMath.h -- SIMD C++ Math library
    //
    // Copyright (c) Microsoft Corporation.
    // Licensed under the MIT License.
    //
    // http://go.microsoft.com/fwlink/?LinkID=615560
    //-------------------------------------------------------------------------------------
    [[nodiscard]] constexpr float Cos(RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / PI * 2.0f * rad;
        if (rad >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad - PI * 2.0f * quotient;

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

    // From DirectXMath.h
    // 
    //-------------------------------------------------------------------------------------
    // DirectXMath.h -- SIMD C++ Math library
    //
    // Copyright (c) Microsoft Corporation.
    // Licensed under the MIT License.
    //
    // http://go.microsoft.com/fwlink/?LinkID=615560
    //-------------------------------------------------------------------------------------
    BYTEENGINE_API [[nodiscard]] RadianF Asin(float value) noexcept;

    BYTEENGINE_API [[nodiscard]] RadianD Asin(double value);

    // From DirectXMath.h
    // 
    //-------------------------------------------------------------------------------------
    // DirectXMath.h -- SIMD C++ Math library
    //
    // Copyright (c) Microsoft Corporation.
    // Licensed under the MIT License.
    //
    // http://go.microsoft.com/fwlink/?LinkID=615560
    //-------------------------------------------------------------------------------------
    BYTEENGINE_API [[nodiscard]] RadianF Acos(float value) noexcept;

    BYTEENGINE_API [[nodiscard]] RadianD Acos(double value);

    template<FloatingPointNumber T>
    [[nodiscard]] RadianT<T> Atan(T value) { return std::atan(value); }

    // From DirectXMath.h
    // 
    //-------------------------------------------------------------------------------------
    // DirectXMath.h -- SIMD C++ Math library
    //
    // Copyright (c) Microsoft Corporation.
    // Licensed under the MIT License.
    //
    // http://go.microsoft.com/fwlink/?LinkID=615560
    //-------------------------------------------------------------------------------------
    template<FloatingPointNumber T>
    [[nodiscard]] constexpr void SinCos(T& sin, T& cos, RadianT<T> rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        T quotient = 1.0f / (PI * 2.0f) * rad;
        if (rad >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        T y = rad - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        T sign;
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

        T y2 = y * y;

        // 11-degree minimax approximation
        sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        T p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        cos = sign * p;
    }

    template<FloatingPointNumber T, FloatingPointNumber U>
    [[nodiscard]] RadianT<std::common_type_t<T, U>> Atan2(T x, U y) { return std::atan2(x, y); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Sqrt(T value) { return std::sqrt(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Abs(T value) noexcept { return std::abs(value); }

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
    [[nodiscard]] constexpr T Round(T value) noexcept { return std::round(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Ceil(T value) noexcept { return std::ceil(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Floor(T value) noexcept { return std::floor(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Sign(T value) noexcept { return static_cast<T>((value > 0) - (value < 0)); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Fract(T value) noexcept { return value - Floor(value); }

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

    [[nodiscard]] constexpr bool IsEqualApproximetly(float right, float left, float tolerance = Epsilon) noexcept { return Abs(left - right) < tolerance; }
    [[nodiscard]] constexpr bool IsEqualApproximetly(double right, double left, double tolerance = EpsilonD) noexcept { return Abs(left - right) < tolerance; }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto LoopValue(T t, U start, V end)
    {
        using Common = std::common_type_t<T, U, V>;

        Common length = end - start;
        Common offsetValue = t - start;

        return start + (offsetValue - (Floor(offsetValue / length) * length) + (offsetValue < 0.0f ? length : 0.0f));
    }

    template<AnyNumber T, AnyNumber U>
        requires Internal::AnyFloating<T, U>
    [[nodiscard]] constexpr auto PingPong(T t, U length) noexcept
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
    [[nodiscard]] constexpr auto MoveTowards(T current, U target, V maxDelta) noexcept
    {
        if (Abs(target - current) <= maxDelta)
            return target;

        return current + Sign(target - current) * maxDelta;
    }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto SmoothStep(T from, U to, V delta) noexcept { return Abs(to - from) <= delta ? to : from + Sign(to - from) * delta; }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsFinite(T value) noexcept { return std::isfinite(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsNaN(T value) noexcept { return std::isnan(value); }

    template<AnyNumber T, AnyNumber U, AnyNumber V, AnyNumber W, AnyNumber X>
    [[nodiscard]] constexpr auto Remap(T value, U oldStart, V oldEnd, W newStart, X newEnd) noexcept
    {
        return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
    }

    template<std::forward_iterator It>
        requires (std::is_convertible_v<std::iter_value_t<It>, float> && (sizeof(std::iter_value_t<It>) <= sizeof(float)))
    [[nodiscard]] constexpr float Average(It begin, It end)
    {
        float sum = 0.0f;
        int32 amount = 0;

        while (++begin != end)
        {
            sum += static_cast<float>(*begin);
            amount++;
        }

        if (amount == 0)
            return 0.0f;

        return sum / amount;
    }

    template<std::forward_iterator It>
        requires std::is_convertible_v<std::iter_value_t<It>, double> && (sizeof(std::iter_value_t<It>) == sizeof(double))
    [[nodiscard]] constexpr double Average(It begin, It end)
    {
        double sum = 0.0;
        int32 amount = 0;

        while (++begin != end)
        {
            sum += static_cast<double>(*begin);
            amount++;
        }

        if (amount == 0)
            return 0.0;

        return sum / amount;
    }
}
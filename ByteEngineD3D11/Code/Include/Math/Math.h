#pragma once

#include <cmath>
#include <concepts>
#include <iterator>
#include <limits>
#include <cassert>

#undef min
#undef max

#include "Primitives.h"

namespace ByteEngine::Math
{

    using RadiansF = float;
    using RadiansD = double;
    using DegreesF = float;
    using DegreesD = double;
}

namespace ByteEngine::Math::Math
{
    template<typename T>
    concept FloatingPointNumber = std::floating_point<T> && !std::same_as<T, long double>;

    template<typename T>
    concept IntegerNumber = std::integral<T> && !std::is_same_v<T, bool>;

    template<typename T>
    concept AnyNumber = IntegerNumber<T> || FloatingPointNumber<T>;

    template <typename It, typename T>
    concept ForwardIteratorTo = std::forward_iterator<It> && std::same_as<typename std::iterator_traits<It>::value_type, T>;

    constexpr float Infinity = std::numeric_limits<float>::infinity();
    constexpr double InfinityD = std::numeric_limits<double>::infinity();
    constexpr float NegativeInfinity = -std::numeric_limits<float>::infinity();
    constexpr double NegativeInfinityD = -std::numeric_limits<double>::infinity();

    constexpr float PI = 3.141592654f;
    constexpr double PID = 3.141592653589793;

    constexpr float Epsilon = 1e-5f;
    constexpr RadiansF AngleEpsilon = 1e-4f;
    constexpr float UnitSizeEpsilon = 1e-4f;

    constexpr RadiansF DegToRad(DegreesF deg) noexcept { return deg * (PI / 180.0f); }
    constexpr RadiansD DegToRad(DegreesD deg) noexcept { return deg * (PID / 180.0); }

    constexpr DegreesF RadToDeg(RadiansF rad) noexcept { return rad * (180.0f / PI); }
    constexpr DegreesD RadToDeg(RadiansD rad) noexcept { return rad * (180.0 / PID); }

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
    [[nodiscard]] constexpr float Sin(RadiansF rad) noexcept
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

    [[nodiscard]] inline double Sin(RadiansD rad) { return std::sin(rad); }

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
    [[nodiscard]] constexpr float Cos(RadiansF rad) noexcept
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

    [[nodiscard]] inline double Cos(RadiansD rad) { return std::cos(rad); }

    [[nodiscard]] inline float Tan(RadiansF rad) noexcept { return std::tan(rad); }
    [[nodiscard]] inline double Tan(RadiansD rad) { return std::tan(rad); }

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
    [[nodiscard]] inline RadiansF Asin(float value) noexcept
    {
        // Clamp input to [-1,1].
        bool nonnegative = (value >= 0.0f);
        float x = fabsf(value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;  // acos(|x|)

        // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
        return (nonnegative ? PI / 2.0f - result : result - PI / 2.0f);
    }

    [[nodiscard]] inline RadiansD Asin(double value) { return std::asin(value); }

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
    [[nodiscard]] inline RadiansF Acos(float value) noexcept
    {
        // Clamp input to [-1,1].
        bool nonnegative = (value >= 0.0f);
        float x = fabsf(value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;

        // acos(x) = pi - acos(-x) when x < 0
        return (nonnegative ? result : PI - result);
    }

    [[nodiscard]] inline RadiansD Acos(double value) { return std::acos(value); }

    [[nodiscard]] inline RadiansF Atan(float value) noexcept { return std::atan(value); }
    [[nodiscard]] inline RadiansD Atan(double value) { return std::atan(value); }

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
    [[nodiscard]] constexpr void SinCos(float& sin, float& cos, RadiansF rad) noexcept
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

    [[nodiscard]] inline RadiansF Atan2(float x, float y) noexcept { return std::atan2(y, x); }
    [[nodiscard]] inline RadiansD Atan2(double x, double y) { return std::atan2(y, x); }

    [[nodiscard]] inline float Sqrt(float value) noexcept { return std::sqrt(value); }
    [[nodiscard]] inline double Sqrt(double value) { return std::sqrt(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Abs(T value) noexcept { return std::abs(value); }

    template<AnyNumber T, AnyNumber U, AnyNumber V>
    [[nodiscard]] constexpr decltype(T(1)* U(1)* V(1)) Clamp(T value, U min, V max) noexcept
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
    [[nodiscard]] constexpr int32 RoundToInt32(float value) noexcept { return static_cast<int32>(Round(value)); }
    [[nodiscard]] constexpr int64 RoundToInt64(double value) noexcept { return static_cast<int64>(Round(value)); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Ceil(T value) noexcept { return std::ceil(value); }
    [[nodiscard]] constexpr int32 CeilToInt32(float value) noexcept { return static_cast<int32>(Ceil(value)); }
    [[nodiscard]] constexpr int64 CeilToInt64(double value) noexcept { return static_cast<int64>(Ceil(value)); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Floor(T value) noexcept { return std::floor(value); }
    [[nodiscard]] constexpr int32 FloorToInt32(float value) noexcept { return static_cast<int32>(Floor(value)); }
    [[nodiscard]] constexpr int64 FloorToInt64(double value) noexcept { return static_cast<int64>(Floor(value)); }

    template<AnyNumber T>
    [[nodiscard]] constexpr T Sign(T value) noexcept { return static_cast<T>((value > 0) - (value < 0)); }

    template<FloatingPointNumber T>
    [[nodiscard]] constexpr T Fract(T value) noexcept { return value - Floor(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Exp(T value) noexcept { return std::exp(value); }

    [[nodiscard]] inline float Pow(float value, float power) noexcept { return std::pow(value, power); }
    [[nodiscard]] inline double Pow(double value, double power) noexcept { return std::pow(value, power); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log(T value) noexcept { return std::log(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log10(T value) noexcept { return std::log10(value); }

    template<FloatingPointNumber T>
    [[nodiscard]] inline T Log2(T value) noexcept { return std::log2(value); }

    [[nodiscard]] inline float LogN(float value, float base) noexcept { return Log(value) / Log(base); }
    [[nodiscard]] inline double LogN(double value, double base) noexcept { return Log(value) / Log(base); }

    [[nodiscard]] inline float Fmod(float x, float y) noexcept { return std::fmod(x, y); }
    [[nodiscard]] inline double Fmod(double x, double y) noexcept { return std::fmod(x, y); }

    [[nodiscard]] constexpr bool IsEqualApproximetly(float right, float left, float tolerance = Epsilon) noexcept { return Abs(left - right) < tolerance; }
    [[nodiscard]] constexpr bool IsEqualApproximetly(double right, double left, double tolerance = Epsilon) noexcept { return Abs(left - right) < tolerance; }

    [[nodiscard]] constexpr float LoopValue(float t, float start, float end)
    {
        float length = end - start;
        float offsetValue = t - start;

        return start + (offsetValue - (Floor(offsetValue / length) * length) + (offsetValue < 0 ? length : 0));
    }

    [[nodiscard]] constexpr double LoopValue(double t, double start, double end)
    {
        double length = end - start;
        double offsetValue = t - start;

        return start + (offsetValue - (Floor(offsetValue / length) * length) + (offsetValue < 0 ? length : 0));
    }

    [[nodiscard]] constexpr float PingPong(float t, float length) noexcept { return (length != 0.0f) ? Abs(Fract((t - length) / (length * 2.0f)) * length * 2.0f - length) : 0.0f; }
    [[nodiscard]] constexpr double PingPong(double t, double length) noexcept { return (length != 0.0) ? Abs(Fract((t - length) / (length * 2.0)) * length * 2.0 - length) : 0.0; }

    [[nodiscard]] inline float AngleDifference(float from, float to) noexcept
    {
        float difference = Fmod(to - from, 360.0f);
        return Fmod(2.0f * difference, 360.0f) - difference;
    }

    [[nodiscard]] inline double AngleDifference(double from, double to) noexcept
    {
        double difference = Fmod(to - from, 360.0);
        return Fmod(2.0 * difference, 360.0) - difference;
    }

    [[nodiscard]] constexpr float Lerp(float from, float to, float t) noexcept { return from + (to - from) * t; }
    [[nodiscard]] constexpr double Lerp(double from, double to, double t) noexcept { return from + (to - from) * t; }

    [[nodiscard]] constexpr float LerpClamped(float from, float to, float t) noexcept { return from + (to - from) * Clamp(t); }
    [[nodiscard]] constexpr double LerpClamped(double from, double to, double t) noexcept { return from + (to - from) * Clamp(t); }

    [[nodiscard]] constexpr float InverseLerp(float from, float to, float t) noexcept { return (t - from) / (to - from); }
    [[nodiscard]] constexpr double InverseLerp(double from, double to, double t) noexcept { return (t - from) / (to - from); }

    [[nodiscard]] inline DegreesF LerpAngle(DegreesF from, DegreesF to, DegreesF t) noexcept { return from + AngleDifference(from, to) * t; }
    [[nodiscard]] inline DegreesD LerpAngle(DegreesD from, DegreesD to, DegreesD t) noexcept { return from + AngleDifference(from, to) * t; }

    [[nodiscard]] inline DegreesF LerpAngleClamped(DegreesF from, DegreesF to, DegreesF t) noexcept { return from + AngleDifference(from, to) * Clamp(t); }
    [[nodiscard]] inline DegreesD LerpAngleClamped(DegreesD from, DegreesD to, DegreesD t) noexcept { return from + AngleDifference(from, to) * Clamp(t); }

    [[nodiscard]] constexpr float MoveTowards(float current, float target, float maxDelta) noexcept
    {
        if (Abs(target - current) <= maxDelta)
            return target;

        return current + Sign(target - current) * maxDelta;
    }

    [[nodiscard]] constexpr double MoveTowards(double current, double target, double maxDelta) noexcept
    {
        if (Abs(target - current) <= maxDelta)
            return target;

        return current + Sign(target - current) * maxDelta;
    }

    [[nodiscard]] constexpr float SmoothStep(float from, float to, float delta) noexcept { return Abs(to - from) <= delta ? to : from + Sign(to - from) * delta; }
    [[nodiscard]] constexpr double SmoothStep(double from, double to, double delta) noexcept { return Abs(to - from) <= delta ? to : from + Sign(to - from) * delta; }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsFinite(T value) noexcept { return std::isfinite(value); }

    template<AnyNumber T>
    [[nodiscard]] constexpr bool IsNaN(T value) noexcept { return std::isnan(value); }

    [[nodiscard]] constexpr float Remap(float value, float oldStart, float oldEnd, float newStart, float newEnd) noexcept
    {
        return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
    }

    [[nodiscard]] constexpr double Remap(double value, double oldStart, double oldEnd, double newStart, double newEnd) noexcept
    {
        return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
    }

    template<std::forward_iterator It>
        requires std::is_convertible_v<std::iter_value_t<It>, float> && (sizeof(std::iter_value_t<It>) <= sizeof(float))
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
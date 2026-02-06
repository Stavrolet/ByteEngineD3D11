#include "Math/Math.h"

namespace ByteEngine::Math::Math
{
    double Sin(RadianD rad) { return std::sin(rad); }
    double Cos(RadianD rad) { return std::cos(rad); }

    float Tan(RadianF rad) noexcept { return std::tan(rad); }
    double Tan(RadianD rad) { return std::tan(rad); }

    RadianF Asin(float value) noexcept
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

    RadianD Asin(double value) { return std::asin(value); }

    RadianF Acos(float value) noexcept
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

    RadianD Acos(double value) { return std::acos(value); }

    RadianF Atan(float value) noexcept { return std::atan(value); }
    RadianD Atan(double value) { return std::atan(value); }

    RadianF Atan2(float x, float y) noexcept { return std::atan2(x, y); }
    RadianD Atan2(double x, double y) { return std::atan2(x, y); }

    float Sqrt(float value) noexcept { return std::sqrt(value); }
    double Sqrt(double value) { return std::sqrt(value); }

    float Pow(float value, float power) noexcept { return std::pow(value, power); }
    double Pow(double value, double power) noexcept { return std::pow(value, power); }

    float LogN(float value, float base) noexcept { return Log(value) / Log(base); }
    double LogN(double value, double base) noexcept { return Log(value) / Log(base); }

    float Fmod(float x, float y) noexcept { return std::fmod(x, y); }
    double Fmod(double x, double y) noexcept { return std::fmod(x, y); }

    RadianF AngleDifference(RadianF from, RadianF to) noexcept
    {
        float difference = Fmod(to - from, 2.0f * PI);
        return Fmod(2.0f * difference, 2.0f * PI) - difference;
    }

    RadianD AngleDifference(RadianD from, RadianD to) noexcept
    {
        double difference = Fmod(to - from, 2.0 * PI_D);
        return Fmod(2.0 * difference, 2.0 * PI_D) - difference;
    }

    RadianF LerpAngle(RadianF from, RadianF to, float t) noexcept { return from + AngleDifference(from, to) * t; }
    RadianD LerpAngle(RadianD from, RadianD to, double t) noexcept { return from + AngleDifference(from, to) * t; }

    RadianF LerpAngleClamped(RadianF from, RadianF to, float t) noexcept { return LerpAngle(from, to, Clamp(t)); }
    RadianD LerpAngleClamped(RadianD from, RadianD to, double t) noexcept { return LerpAngle(from, to, Clamp(t)); }
}
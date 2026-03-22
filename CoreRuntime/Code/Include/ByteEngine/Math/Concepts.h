#pragma once

#include <concepts>
#include <type_traits>

namespace ByteEngine::Math
{
    struct RadianF;
    struct RadianD;
    struct DegreeF;
    struct DegreeD;

    template<typename T>
    concept FloatingPointNumber = (std::floating_point<T> && !std::same_as<T, long double>) || std::same_as<T, RadianF> || std::same_as<T, RadianD> || std::same_as<T, DegreeF> || std::same_as<T, DegreeD>;

    template<typename T>
    concept IntegerNumber = std::integral<T> && !std::is_same_v<T, bool>;

    template<typename T>
    concept AnyNumber = IntegerNumber<T> || FloatingPointNumber<T>;
}
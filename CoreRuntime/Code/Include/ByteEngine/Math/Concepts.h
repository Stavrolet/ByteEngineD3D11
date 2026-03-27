#pragma once

#include <concepts>
#include <type_traits>

namespace ByteEngine::Math
{
    template<typename T>
    concept FloatingPointNumber = std::floating_point<T> && !std::same_as<T, long double>;

    template<typename T>
    concept IntegerNumber = std::integral<T> && !std::is_same_v<T, bool>;

    template<typename T>
    concept AnyNumber = IntegerNumber<T> || FloatingPointNumber<T>;
}
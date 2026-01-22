#pragma once

#include <type_traits>
#include "Primitives.h"

namespace ByteEngine
{
    template<typename T>
    concept EnumType = std::is_enum_v<T>;

    template<typename T>
    concept SuitableForBitOps = (std::is_enum_v<T> || std::integral<T>) && !std::is_same_v<T, bool>;

    template<EnumType T>
    constexpr std::underlying_type_t<T> ToUnderlying(T value)
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    template<EnumType T>
    constexpr T operator|(T a, T b)
    {
        return static_cast<T>(ToUnderlying<T>(a) | ToUnderlying<T>(b));
    }

    template<EnumType T>
    constexpr T& operator|=(T& a, T b)
    {
        a = a | b;
        return a;
    }

    template<EnumType T>
    constexpr T operator&(T a, T b)
    {
        return static_cast<T>(ToUnderlying<T>(a) & ToUnderlying<T>(b));
    }

    template<EnumType T>
    constexpr T& operator&=(T& a, T b)
    {
        a = a & b;
        return a;
    }

    template<EnumType T>
    constexpr T operator~(T a)
    {
        return static_cast<T>(~ToUnderlying<T>(a));
    }

    template<SuitableForBitOps T>
    constexpr bool HasFlags(T value, T flags)
    {
        return (value & flags) == flags;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr bool HasAllFlags(T value, U flags)
    {
        return (value & flags) == flags;
    }

    template<SuitableForBitOps T>
    constexpr bool HasOneFlag(T value, T flags)
    {
        return (value & flags) != 0;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr bool HasOneFlag(T value, U flags)
    {
        return (value & flags) != 0;
    }
}
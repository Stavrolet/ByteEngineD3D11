#pragma once

#include "TypesAliases/PrimitiveTypesAliases.h"
#include <type_traits>

namespace ByteEngine
{
    template<typename T>
    concept EnumType = std::is_enum_v<T>;

    template<EnumType T>
    static constexpr std::underlying_type_t<T> ToUnderlying(T value)
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

    template<typename T> 
        requires std::is_enum_v<T> || std::integral<T>
    constexpr bool HasFlags(T value, T flags)
    {
        return (value & flags) == flags;
    }

    template<typename T, typename U>
        requires ((std::is_enum_v<T> || std::integral<T>) && !std::is_same_v<T, bool>) && ((std::is_enum_v<U> || std::integral<U>) && !std::is_same_v<U, bool>)
    constexpr bool HasAllFlags(T value, U flags)
    {
        return (value & flags) == flags;
    }

    template<typename T> 
        requires std::is_enum_v<T> || std::integral<T>
    constexpr bool HasOneFlag(T value, T flags)
    {
        return (value & flags) != 0;
    }

    template<typename T, typename U>
        requires ((std::is_enum_v<T> || std::integral<T>) && !std::is_same_v<T, bool>) && ((std::is_enum_v<U> || std::integral<U>) && !std::is_same_v<U, bool>)
    constexpr bool HasOneFlag(T value, U flags)
    {
        return (value & flags) != 0;
    }
}
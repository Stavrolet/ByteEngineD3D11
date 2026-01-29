#pragma once

#include <type_traits>
#include "Primitives.h"

namespace ByteEngine
{
    template<typename T>
    concept SuitableForBitOps = (std::is_enum_v<T> || std::integral<T>) && !std::is_same_v<T, bool>;

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr bool HasAllFlags(T value, U flags)
    {
        static_assert(sizeof(U) <= sizeof(T), "Flags mask is greater then bit-field");
        return (value & flags) == flags;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr bool HasOneFlag(T value, U flags)
    {
        static_assert(sizeof(U) <= sizeof(T), "Flags mask is greater then bit-field");
        return (value & flags) != 0;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr void SetFlags(T& bitField, U flags)
    {
        static_assert(sizeof(U) <= sizeof(T), "Flags mask is greater then bit-field");
        bitField |= flags;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
    constexpr void ClearFlags(T& bitField, U flags)
    {
        static_assert(sizeof(U) <= sizeof(T), "Flags mask is greater then bit-field");
        bitField &= ~flags;
    }
}
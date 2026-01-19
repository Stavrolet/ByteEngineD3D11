#pragma once

#include <source_location>
#include <format>
#include <type_traits>

#include "TypesAliases/PrimitiveTypesAliases.h"
#include "TypesAliases/StdStringViewTypeAliases.h"

namespace ByteEngine::DebugHelper
{
    struct FmtWithLocation
    {
        StringView fmt;
        std::source_location loc;

        template <typename T> requires std::convertible_to<T, std::string_view>
        FmtWithLocation(const T& s, std::source_location l = std::source_location::current())
            : fmt(s), loc(l)
        { }
    };

    void LogCriticalError(StringView errorMessageForUser, uint32 errorCode, const ::std::source_location& loc = ::std::source_location::current());
    void LogDebugError(uint32 errorCode, const ::std::source_location& loc = ::std::source_location::current());
    void LogDebugMessageInternal(FmtWithLocation fmt, std::format_args args);

    template <typename... Args>
    inline void LogDebugMessage(FmtWithLocation fmt, Args&&... args)
    {
        LogDebugMessageInternal(fmt, std::make_format_args(std::forward<Args>(args)...));
    }
}
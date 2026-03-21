#pragma once

#include <format>
#include <source_location>
#include <string_view>
#include <type_traits>

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Primitives.h"

namespace ByteEngine::DebugHelper
{
    struct FmtWithLocation
    {
        std::string_view fmt;
        std::source_location loc;

        template <typename T> 
            requires std::convertible_to<T, std::string_view>
        constexpr FmtWithLocation(const T& s, std::source_location l = std::source_location::current())
            : fmt(s), loc(l)
        { }
    };

    [[noreturn]] BYTEENGINE_API void LogCriticalError(std::string_view errorMessageForUser, uint32 errorCode, const ::std::source_location& loc = ::std::source_location::current());
    BYTEENGINE_API void LogDebugError(uint32 errorCode, const ::std::source_location& loc = ::std::source_location::current());
    BYTEENGINE_API void LogDebugMessageInternal(FmtWithLocation fmt, std::format_args args);

    template <typename... Args>
    inline void LogDebugMessage(FmtWithLocation fmt, Args&&... args)
    {
        LogDebugMessageInternal(fmt, std::make_format_args(std::forward<Args>(args)...));
    }
}
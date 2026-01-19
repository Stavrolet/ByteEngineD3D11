#pragma once

#include <string_view>

namespace ByteEngine
{
    using StringView = std::string_view;
    using StringWView = std::wstring_view;
    using String8View = std::u8string_view;
    using String16View = std::u16string_view;
    using String32View = std::u32string_view;
}
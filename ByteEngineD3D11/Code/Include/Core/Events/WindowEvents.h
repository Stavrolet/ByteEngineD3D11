#pragma once

#include <variant>

#include "Primitives.h"
#include "Core/Base/WindowMode.h"

namespace ByteEngine
{
    struct WindowResizeEvent
    {
        int16 width;
        int16 height;
    };

    struct WindowModeChangeEvent
    {
        WindowMode mode;
    };

    struct WindowLoseFocusEvent { };
    struct WindowGetFocusEvent { };
    struct WindowCloseEvent { };

    using Event = std::variant<WindowResizeEvent, WindowModeChangeEvent, WindowLoseFocusEvent, WindowGetFocusEvent, WindowCloseEvent>;
}
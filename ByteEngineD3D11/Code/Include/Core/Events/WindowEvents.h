#pragma once

#include "Primitives.h"
#include "Core/Base/Window/WindowMode.h"

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
}
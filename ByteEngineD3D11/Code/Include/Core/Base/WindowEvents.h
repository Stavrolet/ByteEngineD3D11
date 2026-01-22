#pragma once

#include "Primitives.h"

namespace ByteEngine
{
    enum class WindowEvents : uint8
    {
        NONE = 0x0,
        RESIZE = 0x1,
        WINDOW_MODE_CHANGED = 0x2,
        LOST_FOCUS = 0x4,
        GAINED_FOCUS = 0x8,
        CLOSE = 0x10,
        PRINT_SWAP_CHAIN_FULLSCREEN_STATE = 0x20 // this is a temporary solution
    };
}
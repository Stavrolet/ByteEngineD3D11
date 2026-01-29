#pragma once

#include <variant>

#include "Core/Events/InputEvents.h"
#include "Core/Events/WindowEvents.h"

namespace ByteEngine
{
    constexpr int32 WINDOW_RESIZE_EVENT_INDEX = 0;
    constexpr int32 WINDOW_MODE_CHANGE_EVENT_INDEX = 1;
    constexpr int32 WINDOW_LOSE_FOCUS_EVENT_INDEX = 2;
    constexpr int32 WINDOW_GET_FOCUS_EVENT_INDEX = 3;
    constexpr int32 WINDOW_CLOSE_EVENT_INDEX = 4;
    constexpr int32 KEY_EVENT_INDEX = 5;
    constexpr int32 MOUSE_MOVE_EVENT_INDEX = 6;
    constexpr int32 MOUSE_WHEEL_EVENT_INDEX = 7;

    using Event = std::variant<WindowResizeEvent, WindowModeChangeEvent, WindowLoseFocusEvent, WindowGetFocusEvent, WindowCloseEvent, KeyEvent, MouseMoveEvent, MouseWheelEvent>;
}
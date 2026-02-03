#pragma once

#include "Core/Input/KeyCode.h"
#include "Math/Vector2.h"

namespace ByteEngine
{
    using namespace ::ByteEngine::Math;

    struct KeyEvent
    {
        KeyCode code;
        bool isPressed;
    };

    struct MouseMoveEvent
    {
        Vector2 delta;
    };

    struct MouseWheelEvent
    {
        float horizontalDelta;
        float verticalDelta;
    };
}
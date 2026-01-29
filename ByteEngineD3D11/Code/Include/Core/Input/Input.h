#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Core/Base/Singleton.h"
#include "Core/Events/InputEvents.h"
#include "Core/Input/KeyCode.h"
#include "Math/Vector2.h"

namespace ByteEngine
{
    class Input : public Singleton<Input>
    {
        friend class Application;

        struct StringHash
        {
            using is_transparent = void;

            size_t operator()(std::string_view sv) const noexcept
            {
                return std::hash<std::string_view>{}(sv);
            }
        };

        struct StringEqual
        {
            using is_transparent = void;

            bool operator()(std::string_view a, std::string_view b) const noexcept
            {
                return a == b;
            }
        };

    private:
        std::unordered_map<std::string, std::vector<KeyCode>, StringHash, StringEqual> actions;

        std::unordered_map<KeyCode, bool> keysState;
        std::unordered_map<KeyCode, bool> previousFrameKeysState;
        bool isAnyKeyPressed = false;

        Vector2 mouseDelta;
        float horizontalWheelDelta = 0.0f;
        float verticalWheelDelta = 0.0f;

        Input();

    public:
        bool IsActionPressed(std::string_view actionName) const;
        bool IsActionJustPressed(std::string_view actionName) const;
        bool IsActionJustReleased(std::string_view actionName) const;

        bool IsKeyPressed(KeyCode code) const { return keysState.at(code); }
        bool IsKeyJustPressed(KeyCode code) const { return !previousFrameKeysState.at(code) && keysState.at(code); }
        bool IsKeyJustReleased(KeyCode code) const { return previousFrameKeysState.at(code) && !keysState.at(code); }
        bool IsAnyKeyPressed() const { return isAnyKeyPressed; }

        Vector2 GetMouseDelta() const { return mouseDelta; }
        Vector2 GetMousePosition() const;

    private:
        void Update();

        void ProcessKeyEvent(const KeyEvent& event) 
        { 
            keysState[event.code] = event.isPressed;
            isAnyKeyPressed = true;
        }

        void ProcessMouseWheelEvent(const MouseWheelEvent& event)
        {
            horizontalWheelDelta += event.horizontalDelta;
            verticalWheelDelta += event.verticalDelta;
        }

        void ProcessMouseMoveEvent(const MouseMoveEvent& event) { mouseDelta += event.delta; }
    };
}
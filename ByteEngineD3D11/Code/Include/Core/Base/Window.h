#pragma once

#include <string_view>
#include <vector>

#include "Primitives.h"
#include "Core/Base/WindowMode.h"
#include "Core/Events/WindowEvents.h"

struct HWND__;
using HWND = HWND__*;
struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;
#ifdef _WIN64
using LRESULT = __int64;
using WPARAM = unsigned __int64;
using LPARAM = __int64;
#else
using LRESULT = long;
using WPARAM = unsigned int;
using LPARAM = long;
#endif
using UINT = unsigned int;

#define WM_MODECHANGE (WM_APP + 1)

namespace ByteEngine
{
    class Window
    {
        friend class Application;

    public:

    private:
        HWND hwnd = nullptr;

        int16 width = 0;
        int16 height = 0;

        bool initialized = false;

        WindowMode mode = WindowMode::BorderlessFullscreen;
        WindowMode previousMode = WindowMode::BorderlessFullscreen;

        std::vector<Event> events;

    public:
        ~Window() { Close(); }

        void Initialize(std::wstring_view windowName, WindowMode initialMode = WindowMode::BorderlessFullscreen, int16 width = 0, int16 height = 0, Window* parent = nullptr);
        void SetWindowMode(WindowMode modeToSet) const;

        void Close();

        HWND GetHwnd() const { return hwnd; }
        int32 GetWidth() const { return width; }
        int32 GetHeight() const { return height; }
        WindowMode GetWindowMode() const { return mode; }

    private:
        static LRESULT __stdcall StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT __stdcall WndProc(UINT message, WPARAM wParam, LPARAM lParam);

        const std::vector<Event>& PollEvents();

        void HandleWindowModeChangeMessage(WindowMode modeToSet);
    };
}
#pragma once

#include "Primitives.h"
#include "Collections/StringView.h"
#include "Core/Base/WindowEvents.h"
#include "Core/Base/WindowMode.h"

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

namespace ByteEngine
{
    class Window
    {
        friend class Application;

    private:
        HWND hwnd = nullptr;

        int32 width = 0;
        int32 height = 0;

        bool initialized = false;

        WindowMode mode = WindowMode::BORDERLESS_FULLSCREEN;
        WindowMode previousMode = WindowMode::BORDERLESS_FULLSCREEN;

        WindowEvents events = WindowEvents::NONE;

    public:
        ~Window() { Close(); }

        void Initialize(StringWView windowName, WindowMode initialMode = WindowMode::BORDERLESS_FULLSCREEN, int32 width = 0, int32 height = 0, Window* parent = nullptr);
        void SetWindowMode(WindowMode modeToSet);

        void Close();

        HWND GetHwnd() const { return hwnd; }
        int32 GetWidth() const { return width; }
        int32 GetHeight() const { return height; }
        WindowMode GetWindowMode() const { return mode; }

    private:
        static LRESULT __stdcall StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT __stdcall WndProc(UINT message, WPARAM wParam, LPARAM lParam);

        WindowEvents PollEvents();
    };
}
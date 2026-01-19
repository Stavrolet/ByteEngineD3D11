#pragma once

#include "TypesAliases/PrimitiveTypesAliases.h"
#include "TypesAliases/StdStringViewTypeAliases.h"
#include "TypesAliases/StdStringTypesAliases.h"
#include "WindowEvents.h"

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
        bool isFullscreen = false;
        bool isMinimized = false;

        WindowEvents events = WindowEvents::NONE;

        WINDOWPLACEMENT previousWindowPlacement = { };

    public:
        ~Window() { Close(); }

        void Initialize(StringWView windowName, bool fullScreen = true, int32 width = 0, int32 height = 0, Window* parent = nullptr);
        void SetFullscreenState(bool fullscreen);

        void Close();

        HWND GetHwnd() const { return hwnd; }
        int32 GetWidth() const { return width; }
        int32 GetHeight() const { return height; }
        bool IsFullscreen() const { return isFullscreen; }
        bool IsMinimized() const { return isMinimized; }

    private:
        static LRESULT __stdcall StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT __stdcall WndProc(UINT message, WPARAM wParam, LPARAM lParam);

        WindowEvents PollEvents();
        void SetPreviousPlacement() const;
    };
}
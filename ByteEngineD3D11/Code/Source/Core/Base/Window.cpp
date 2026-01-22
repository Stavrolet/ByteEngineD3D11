#include "WinApiExcludingDefs/LeanAndMean.h"
#undef NOSYSMETRICS
#undef NOVIRTUALKEYCODES

#include <Windows.h>
#include <cmath>

#include "Core/Base/Window.h"
#include "DebugLogHelper.h"
#include "Utilities/BitFlagsHelper.h"
#include "Core/Base/Application.h"
#include "Resource.h"

using namespace ByteEngine;

void Window::Initialize(StringWView windowName, WindowMode initialMode, int32 width, int32 height, Window* parent)
{
    mode = initialMode;

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    if (hInstance == nullptr)
    {
        DebugHelper::LogCriticalError("Failed to create the program Window", GetLastError());
        return;
    }

    WNDCLASSEX winClass = { };
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = StaticWndProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BYTEENGINED3D11));
    winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    winClass.lpszClassName = windowName.data();
    winClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (RegisterClassEx(&winClass) == 0)
    {
        DebugHelper::LogCriticalError("Failed to create the application Window", GetLastError());
        return;
    }

    DWORD initialStyle = 0;
    initialStyle |= initialMode == WindowMode::WINDOWED || initialMode == WindowMode::MAXIMIZED ? WS_OVERLAPPEDWINDOW : (WS_POPUP | WS_VISIBLE);

    if (initialMode == WindowMode::BORDERLESS_FULLSCREEN || initialMode == WindowMode::EXCLUSIVE_FULLSCREEN)
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    }

    int32 initialPosX = 0;
    int32 initialPosY = 0;

    if (initialMode == WindowMode::WINDOWED || initialMode == WindowMode::MAXIMIZED)
    {
        initialPosX = CW_USEDEFAULT;
        initialPosY = CW_USEDEFAULT;
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        reinterpret_cast<LPCWSTR>(windowName.data()),
        reinterpret_cast<LPCWSTR>(windowName.data()),
        initialStyle,
        initialPosX, initialPosY, width, height,
        parent != nullptr ? parent->GetHwnd() : nullptr,
        nullptr, hInstance, this
    );

    if (hwnd == nullptr)
    {
        DebugHelper::LogCriticalError("Failed to create the program Window", GetLastError());
        return;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);

    this->width = width;
    this->height = height;

    initialized = true;
}

void Window::SetWindowMode(WindowMode modeToSet)
{
    uint8 a = static_cast<uint8>(mode);
    uint8 b = static_cast<uint8>(modeToSet);

    DebugHelper::LogDebugMessage("SetWindowMode called. Current mode = {}, mode to set = {}", a, b);

    if (mode == modeToSet)
        return;

    switch (modeToSet)
    {
    case WindowMode::WINDOWED:
    case WindowMode::MAXIMIZED:
        if (mode == WindowMode::BORDERLESS_FULLSCREEN || mode == WindowMode::EXCLUSIVE_FULLSCREEN)
        {
            DWORD style = static_cast<DWORD>(GetWindowLongPtr(hwnd, GWL_STYLE));

            style &= ~(WS_POPUP | WS_VISIBLE);
            style |= WS_OVERLAPPEDWINDOW;

            if (SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW) == 0)
            {
                DebugHelper::LogDebugError(GetLastError());
                return;
            }
        }

        ShowWindow(hwnd, modeToSet == WindowMode::WINDOWED ? SW_RESTORE : SW_MAXIMIZE);
        break;
    case WindowMode::BORDERLESS_FULLSCREEN:
    case WindowMode::EXCLUSIVE_FULLSCREEN:
        if (mode == WindowMode::WINDOWED || mode == WindowMode::MAXIMIZED)
        {
            MONITORINFO monInfo = { };
            monInfo.cbSize = sizeof(MONITORINFO);

            int32 width = 0;
            int32 height = 0;
            int32 posX = 0;
            int32 posY = 0;

            if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &monInfo) != 0)
            {
                width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
                height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
                posX = monInfo.rcMonitor.left;
                posY = monInfo.rcMonitor.top;
            }
            else
            {
                DebugHelper::LogDebugError(GetLastError());
                width = GetSystemMetrics(SM_CXSCREEN);
                height = GetSystemMetrics(SM_CYSCREEN);
            }

            DWORD style = static_cast<DWORD>(GetWindowLongPtr(hwnd, GWL_STYLE));

            style &= ~WS_OVERLAPPEDWINDOW;
            style |= WS_POPUP;

            if (SetWindowLongPtr(hwnd, GWL_STYLE, style) == 0)
            {
                DebugHelper::LogDebugError(GetLastError());
                return;
            }

            if (SetWindowPos(hwnd, nullptr, posX, posY, width, height, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) == 0)
            {
                DebugHelper::LogDebugError(GetLastError());
                return;
            }
        }
        break;
    case WindowMode::MINIMIZED:
        ShowWindow(hwnd, SW_MINIMIZE);
        break;
    }

    mode = modeToSet;
    events |= WindowEvents::WINDOW_MODE_CHANGED;
}

WindowEvents Window::PollEvents()
{
    events = WindowEvents::NONE;

    MSG msg = { };
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            events = WindowEvents::CLOSE;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return events;
}

void Window::Close()
{
    if (hwnd != nullptr)
    {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

LRESULT Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* window = nullptr;

    if (msg == WM_CREATE)
    {
        CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<Window*>(createStruct->lpCreateParams);
        SetLastError(0);
        
        if (SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window)) == 0 && GetLastError() != 0)
        {
            DebugHelper::LogDebugError(GetLastError());
            MessageBox(nullptr, L"Failed to crate the program Window.", L"Critical Error", MB_OK | MB_ICONERROR);
            return -1;
        }

        return 0;
    }
    else
    {
        window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window != nullptr)
        return window->WndProc(msg, wParam, lParam);

#ifdef _DEBUG
    if (msg != WM_NCCREATE && msg != WM_NCCALCSIZE)
        OutputDebugStringA(std::format("[WARNING] window is nullptr in StaticWndProc. Message = {}.", msg).c_str());
#endif

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        hwnd = nullptr;
        events |= WindowEvents::CLOSE;
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);

        if (initialized) [[likely]]
        {
            if (wParam == SIZE_MINIMIZED)
            {
                previousMode = mode;
                mode = WindowMode::MINIMIZED;
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                mode = WindowMode::MAXIMIZED;
                DebugHelper::LogDebugMessage("WM_SIZE received. wParam = SIZE_MAXIMIZED");
            }
            else if (wParam == SIZE_RESTORED)
            {
                mode = WindowMode::WINDOWED;
            }
        }

        events |= WindowEvents::RESIZE;
        return 0;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            DebugHelper::LogDebugMessage("Window lost focus");

            events &= ~WindowEvents::GAINED_FOCUS;
            events |= WindowEvents::LOST_FOCUS;

            if (mode == WindowMode::EXCLUSIVE_FULLSCREEN)
                SetWindowMode(WindowMode::MINIMIZED);
        }
        else
        {
            DebugHelper::LogDebugMessage("Window gained focus");

            events &= ~WindowEvents::LOST_FOCUS;
            events |= WindowEvents::GAINED_FOCUS;

            if (mode == WindowMode::MINIMIZED)
                SetWindowMode(previousMode);
        }

        return 0;
    case WM_CLOSE:
        events |= WindowEvents::CLOSE;
        return 0;
    case WM_DISPLAYCHANGE:
        InvalidateRect(hwnd, nullptr, true);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            Application::GetInstance().Quit(0);
        else if (wParam == '1')
            SetWindowMode(WindowMode::WINDOWED);
        else if (wParam == '2')
            SetWindowMode(WindowMode::MAXIMIZED);
        else if (wParam == '3')
            SetWindowMode(WindowMode::BORDERLESS_FULLSCREEN);
        else if (wParam == '4')
            SetWindowMode(WindowMode::EXCLUSIVE_FULLSCREEN);
        else if (wParam == 'P') // this is a temporary solution
            events |= WindowEvents::PRINT_SWAP_CHAIN_FULLSCREEN_STATE;
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
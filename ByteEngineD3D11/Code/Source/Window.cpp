#include "WinApiExcludingDefs/LeanAndMean.h"
#undef NOSYSMETRICS
#undef NOVIRTUALKEYCODES

#include <Windows.h>
#include <cmath>

#include "Window.h"
#include "DebugLogHelper.h"
#include "EnumFlagsHelpers.h"
#include "Application.h"
#include "Resource.h"

using namespace ByteEngine;

void Window::Initialize(StringWView windowName, bool fullScreen, int32 width, int32 height, Window* parent)
{
    isFullscreen = fullScreen;

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

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        reinterpret_cast<LPCWSTR>(windowName.data()),
        reinterpret_cast<LPCWSTR>(windowName.data()),
        fullScreen ? WS_POPUP | WS_VISIBLE : WS_OVERLAPPEDWINDOW,
        fullScreen ? 0 : CW_USEDEFAULT, fullScreen ? 0 : CW_USEDEFAULT,
        fullScreen ? GetSystemMetrics(SM_CXSCREEN) : width,
        fullScreen ? GetSystemMetrics(SM_CYSCREEN) : height,
        parent != nullptr ? parent->GetHwnd() : nullptr,
        nullptr, hInstance, this
    );

    if (hwnd == nullptr)
    {
        DebugHelper::LogCriticalError("Failed to create the program Window", GetLastError());
        return;
    }

    GetWindowPlacement(hwnd, &previousWindowPlacement);
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

    this->width = width;
    this->height = height;
}

void Window::SetFullscreenState(bool fullscreen)
{
    DebugHelper::LogDebugMessage("SetFullScreenState request. Requested state: {}, current state: {}", fullscreen, isFullscreen);

    if (isFullscreen == fullscreen)
        return;

    isFullscreen = fullscreen;

    if (isFullscreen)
    {
        MONITORINFO monInfo = { };
        monInfo.cbSize = sizeof(MONITORINFO);

        if (GetWindowPlacement(hwnd, &previousWindowPlacement) == 0)
        {
            DebugHelper::LogDebugError(GetLastError());
            return;
        }

        if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monInfo) == 0)
        {
            DebugHelper::LogDebugError(GetLastError());

            monInfo.rcMonitor.right = GetSystemMetrics(SM_CXSCREEN);
            monInfo.rcMonitor.left = 0;
            monInfo.rcMonitor.bottom = GetSystemMetrics(SM_CYSCREEN);
            monInfo.rcMonitor.top = 0;
        }

        if (SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP) == 0)
        {
            DebugHelper::LogDebugError(GetLastError());
            return;
        }

        if (SetWindowPos(hwnd, nullptr, monInfo.rcMonitor.left, monInfo.rcMonitor.top, monInfo.rcMonitor.right - monInfo.rcMonitor.left, monInfo.rcMonitor.bottom - monInfo.rcMonitor.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) == 0)
        {
            DebugHelper::LogDebugError(GetLastError());
            return;
        }
    }
    else
    {
        if (SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW) == 0)
        {
            DebugHelper::LogDebugError(GetLastError());
            return;
        }

        if (SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
            DebugHelper::LogDebugError(GetLastError());
    }

    events |= WindowEvents::FULLSCREEN_STATE_CHANGED;
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

void Window::SetPreviousPlacement() const
{ 
    if (!isFullscreen && HasFlags(events, WindowEvents::FULLSCREEN_STATE_CHANGED))
    {
        if (SetWindowPlacement(hwnd, &previousWindowPlacement) == 0)
            DebugHelper::LogDebugError(GetLastError());
    }
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
        OutputDebugStringA(std::format("[WARNING] window is nullptr in StaticWndProc. Message: {}.", msg).c_str());
#endif

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            Application::GetInstance().Quit(0);
        else if (wParam == 'F')
            SetFullscreenState(!isFullscreen);
        return 0;
    case WM_DISPLAYCHANGE:
        InvalidateRect(hwnd, nullptr, true);
        return 0;
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);

        if (wParam == SIZE_MINIMIZED || wParam == SIZE_MAXHIDE)
            isMinimized = true;
        else if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
            isMinimized = false;
        
        events |= WindowEvents::RESIZE;
        return 0;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE && isFullscreen)
        {
            DebugHelper::LogDebugMessage("Window gained focus");

            events &= ~WindowEvents::GAINED_FOCUS;
            events |= WindowEvents::LOST_FOCUS;
            isMinimized = true;
            ShowWindow(hwnd, SW_MINIMIZE);
        }
        else
        {
            DebugHelper::LogDebugMessage("Window lost focus");

            events &= ~WindowEvents::LOST_FOCUS;
            events |= WindowEvents::GAINED_FOCUS;

            if (isMinimized)
                ShowWindow(hwnd, SW_MAXIMIZE);

            isMinimized = false;
        }

        return 0;
    case WM_CLOSE:
        events |= WindowEvents::CLOSE;
        return 0;
    case WM_DESTROY:
        hwnd = nullptr;
        events |= WindowEvents::CLOSE;
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#include "WinApiExcludingDefs/LeanAndMean.h"
#undef NOSYSMETRICS
#undef NOVIRTUALKEYCODES

#include <cmath>
#include <Windows.h>
#include <hidusage.h>

#include "Core/Base/Application.h"
#include "Core/Base/Window/Window.h"
#include "Utilities/BitFlagsHelper.h"
#include "DebugLogHelper.h"
#include "Resource.h"

using namespace ByteEngine;

void Window::Initialize(std::wstring_view windowName, WindowMode initialMode, int16 width, int16 height, Window* parent)
{
    mode = initialMode;

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    if (hInstance == nullptr)
        DebugHelper::LogCriticalError("Failed to create the program Window", GetLastError());

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
        DebugHelper::LogCriticalError("Failed to create the application Window", GetLastError());

    DWORD initialStyle = 0;
    initialStyle |= initialMode == WindowMode::Windowed || initialMode == WindowMode::Mazimized ? WS_OVERLAPPEDWINDOW : (WS_POPUP | WS_VISIBLE);

    if (initialMode == WindowMode::BorderlessFullscreen || initialMode == WindowMode::ExclusiveFullscreen)
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    }

    int32 initialPosX = 0;
    int32 initialPosY = 0;

    if (initialMode == WindowMode::Windowed || initialMode == WindowMode::Mazimized)
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
        DebugHelper::LogCriticalError("Failed to create the program Window", GetLastError());

    ShowWindow(hwnd, SW_MAXIMIZE);

    this->width = width;
    this->height = height;

    RAWINPUTDEVICE rids[] =
    {
        {
            .usUsagePage = HID_USAGE_PAGE_GENERIC,
            .usUsage = HID_USAGE_GENERIC_KEYBOARD,
            .dwFlags = 0,
            .hwndTarget = hwnd
        },
        {
            .usUsagePage = HID_USAGE_PAGE_GENERIC,
            .usUsage = HID_USAGE_GENERIC_MOUSE,
            .dwFlags = 0,
            .hwndTarget = hwnd
        }
    };

    if (RegisterRawInputDevices(rids, 2, sizeof(RAWINPUTDEVICE)) == false)
        DebugHelper::LogCriticalError("Failed to initialize input devices.", GetLastError());

    initialized = true;
}

void Window::SetWindowMode(WindowMode modeToSet) const
{
    uint8 a = static_cast<uint8>(mode);
    uint8 b = static_cast<uint8>(modeToSet);

    DebugHelper::LogDebugMessage("SetWindowMode called. Current mode = {}, mode to set = {}", a, b);

    if (mode == modeToSet)
        return;

    PostMessage(hwnd, WM_MODECHANGE, static_cast<WPARAM>(modeToSet), 0);
}

const std::vector<Event>& Window::PollEvents()
{
    events.clear();

    MSG msg = { };
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            events.push_back(WindowCloseEvent());
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return events;
}

void Window::HandleWindowModeChangeMessage(WindowMode modeToSet)
{
    switch (modeToSet)
    {
    case WindowMode::Windowed:
    case WindowMode::Mazimized:
        if (mode == WindowMode::BorderlessFullscreen || mode == WindowMode::ExclusiveFullscreen)
        {
            DWORD style = static_cast<DWORD>(GetWindowLongPtr(hwnd, GWL_STYLE));

            style &= ~(WS_POPUP | WS_VISIBLE);
            style |= WS_OVERLAPPEDWINDOW;

            if (SetWindowLongPtr(hwnd, GWL_STYLE, style) == 0)
            {
                DebugHelper::LogDebugError(GetLastError());
                return;
            }
        }

        ShowWindow(hwnd, modeToSet == WindowMode::Windowed ? SW_RESTORE : SW_MAXIMIZE);
        break;
    case WindowMode::BorderlessFullscreen:
    case WindowMode::ExclusiveFullscreen:
        if (mode == WindowMode::Windowed || mode == WindowMode::Mazimized)
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
    case WindowMode::Minimized:
        ShowWindow(hwnd, SW_MINIMIZE);
        break;
    }

    mode = modeToSet;
    events.push_back(WindowModeChangeEvent { modeToSet });
}

void Window::HandleRawInputMessage(HRAWINPUT handle)
{
    uint32 size = 0;
    GetRawInputData(handle, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

    std::vector<uint8> buffer(size);
    GetRawInputData(handle, RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));

    RAWINPUT* raw = (RAWINPUT*)buffer.data();

    if (raw->header.dwType == RIM_TYPEKEYBOARD)
    {
        const RAWKEYBOARD& keyboard = raw->data.keyboard;
        uint16 scanCode = 0;
        bool isKeyPressed = !HasOneFlag(keyboard.Flags, (uint16)RI_KEY_BREAK);

        if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE)
            return;

        if (keyboard.VKey == VK_PAUSE)
        {
            events.push_back(KeyEvent { KeyCode::Pause, true });
            return;
        }

        if (keyboard.MakeCode != 0)
        {
            uint16 scanCode = MAKEWORD(
                keyboard.MakeCode & 0x7f,
                (HasOneFlag<uint16, uint16>(keyboard.Flags, RI_KEY_E0) ? 0xe0 : (HasOneFlag<uint16, uint16>(keyboard.Flags, RI_KEY_E1) ? 0xe1 : 0x00))
            );

            events.push_back(KeyEvent { static_cast<KeyCode>(scanCode), isKeyPressed });
        }

        char keyNameBuffer[MAX_PATH] = { };
        GetKeyNameTextA((LONG)MAKELPARAM(0, (HIBYTE(scanCode) ? KF_EXTENDED : 0x00) | LOBYTE(scanCode)), keyNameBuffer, MAX_PATH);

        DebugHelper::LogDebugMessage("Key pressed. Name: {}, is pressed = {}", keyNameBuffer, isKeyPressed);
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        const RAWMOUSE& mouse = raw->data.mouse;

        if (HasOneFlag(mouse.usFlags, (uint16)MOUSE_MOVE_RELATIVE))
            events.push_back(MouseMoveEvent { Vector2(static_cast<float>(mouse.lLastX), static_cast<float>(mouse.lLastY)) });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_WHEEL))
        {
            float delta = static_cast<int16>(mouse.usButtonData) / (float)WHEEL_DELTA;
            events.push_back(MouseWheelEvent { 0.0f, delta });

            if (delta > 0.0f)
                events.push_back(KeyEvent { KeyCode::MouseWheelUp, true });
            else
                events.push_back(KeyEvent { KeyCode::MouseWheelDown, true });
        }
        else if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_HWHEEL))
        {
            float delta = static_cast<int16>(mouse.usButtonData) / (float)WHEEL_DELTA;
            events.push_back(MouseWheelEvent { delta, 0.0f });

            if (delta > 0)
                events.push_back(KeyEvent { KeyCode::MouseWheelRight, true });
            else
                events.push_back(KeyEvent { KeyCode::MouseWheelLeft, true });
        }

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_LEFT_BUTTON_DOWN))
            events.push_back(KeyEvent { KeyCode::MouseLeft, true });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_LEFT_BUTTON_UP))
            events.push_back(KeyEvent { KeyCode::MouseLeft, false });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_RIGHT_BUTTON_DOWN))
            events.push_back(KeyEvent { KeyCode::MouseRight, true });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_RIGHT_BUTTON_UP))
            events.push_back(KeyEvent { KeyCode::MouseRight, false });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_MIDDLE_BUTTON_DOWN))
            events.push_back(KeyEvent { KeyCode::MouseMiddle, true });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_MIDDLE_BUTTON_UP))
            events.push_back(KeyEvent { KeyCode::MouseMiddle, false });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_4_DOWN))
            events.push_back(KeyEvent { KeyCode::MouseExtended1, true });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_4_UP))
            events.push_back(KeyEvent { KeyCode::MouseExtended1, false });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_5_DOWN))
            events.push_back(KeyEvent { KeyCode::MouseExtended2, true });

        if (HasOneFlag(mouse.usButtonFlags, (uint16)RI_MOUSE_BUTTON_5_UP))
            events.push_back(KeyEvent { KeyCode::MouseExtended2, false });
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
        events.push_back(WindowCloseEvent());
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
                mode = WindowMode::Minimized;
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                mode = WindowMode::Mazimized;
                DebugHelper::LogDebugMessage("WM_SIZE received. wParam = SIZE_MAXIMIZED");
            }
            else if (wParam == SIZE_RESTORED)
            {
                mode = WindowMode::Windowed;
            }
        }

        events.push_back(WindowResizeEvent { width, height });
        return 0;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            DebugHelper::LogDebugMessage("Window lost focus");

            events.push_back(WindowLoseFocusEvent());

            if (mode == WindowMode::ExclusiveFullscreen)
                SetWindowMode(WindowMode::Minimized);
        }
        else
        {
            DebugHelper::LogDebugMessage("Window gained focus");

            events.push_back(WindowGetFocusEvent());

            if (mode == WindowMode::Minimized)
                SetWindowMode(previousMode);
        }

        return 0;
    case WM_CLOSE:
        events.push_back(WindowCloseEvent());
        return 0;
    case WM_DISPLAYCHANGE:
        InvalidateRect(hwnd, nullptr, true);
        return 0;
    case WM_INPUT:
        HandleRawInputMessage(reinterpret_cast<HRAWINPUT>(lParam));
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            Application::GetInstance().Quit(0);
        else if (wParam == '1')
            SetWindowMode(WindowMode::Windowed);
        else if (wParam == '2')
            SetWindowMode(WindowMode::Mazimized);
        else if (wParam == '3')
            SetWindowMode(WindowMode::BorderlessFullscreen);
        else if (wParam == '4')
            SetWindowMode(WindowMode::ExclusiveFullscreen);
        return 0;
    case WM_MODECHANGE:
        HandleWindowModeChangeMessage(static_cast<WindowMode>(wParam));
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
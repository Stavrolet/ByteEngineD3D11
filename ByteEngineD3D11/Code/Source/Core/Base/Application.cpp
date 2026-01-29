#define WIN32_LEAN_AND_MEAN
#include <d3d11_1.h>
#include <Windows.h>

#include "Core/Base/Application.h"
#include "Core/Base/Window/Window.h"
#include "Core/Input/Input.h"
#include "Core/Renderer/RenderContext.h"
#include "Utilities/BitFlagsHelper.h"
#include "DebugLogHelper.h"

using namespace ByteEngine;

int32 Application::Run()
{
    Application::SetInstance(this);

    Window gameWindow;
    gameWindow.Initialize(L"ByteEngine D3D11 Application");

    RenderingContext renderingContext;
    renderingContext.Initialize(&gameWindow);
    RenderingContext::SetInstance(&renderingContext);

    Input input;
    Input::SetInstance(&input);

    while (isRunning)
    {
        const std::vector<Event>& events = gameWindow.PollEvents();

        const WindowResizeEvent* windowResizeEvent = nullptr;
        const WindowModeChangeEvent* windowModeChangeEvent = nullptr;
        std::vector<float> backGroundColor{ 0.0f, 0.1f, 0.15f };

        input.Update();

        for (const Event& event : events)
        {
            switch (event.index())
            {
            case WINDOW_RESIZE_EVENT_INDEX:
                windowResizeEvent = &std::get<WINDOW_RESIZE_EVENT_INDEX>(event);
                break;
            case WINDOW_MODE_CHANGE_EVENT_INDEX:
                windowModeChangeEvent = &std::get<WINDOW_MODE_CHANGE_EVENT_INDEX>(event);
                break;
            case KEY_EVENT_INDEX:
            {
                const KeyEvent& e = std::get<KEY_EVENT_INDEX>(event);
                if (e.code == KeyCode::MouseWheelDown || e.code == KeyCode::MouseWheelUp)
                    DebugHelper::LogDebugMessage("-------------Mouse wheel key event");

                input.ProcessKeyEvent(std::get<KEY_EVENT_INDEX>(event));
                break;
            }
            case MOUSE_MOVE_EVENT_INDEX:
                input.ProcessMouseMoveEvent(std::get<MOUSE_MOVE_EVENT_INDEX>(event));
                break;
            case MOUSE_WHEEL_EVENT_INDEX:
                input.ProcessMouseWheelEvent(std::get<MOUSE_WHEEL_EVENT_INDEX>(event));
                break;
            }
        }

        if (Input::GetInstance().IsActionPressed("test1"))
            backGroundColor = { 1.0f, 0.0f, 0.0f };

        renderingContext.OnWindowModeChanged(windowModeChangeEvent);
        renderingContext.OnResize(windowResizeEvent);

        renderingContext.Update(backGroundColor.data());        
    }

    gameWindow.Close();
    DebugHelper::LogDebugMessage("Application is closing");

    return exitCode;
}

void Application::Quit(int32 exitCode)
{
    isRunning = false;
    this->exitCode = exitCode;
}
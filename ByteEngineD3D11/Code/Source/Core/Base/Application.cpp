#include <d3d11_1.h>
#include <Windows.h>

#include "Core/Base/Application.h"
#include "Core/Base/Window.h"
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

    while (isRunning)
    {
        const std::vector<Event>& events = gameWindow.PollEvents();

        const WindowResizeEvent* windowResizeEvent = nullptr;
        const WindowModeChangeEvent* windowModeChangeEvent = nullptr;

        for (const Event& event : events)
        {
            switch (event.index())
            {
            case 0:
                windowResizeEvent = &std::get<0>(event);
                break;
            case 1:
                windowModeChangeEvent = &std::get<1>(event);
                break;
            }
        }

        renderingContext.OnWindowModeChanged(windowModeChangeEvent);
        renderingContext.OnResize(windowResizeEvent);

        renderingContext.OnUpdate();
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
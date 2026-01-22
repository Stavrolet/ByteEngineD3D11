#include <d3d11_1.h>
#include <Windows.h>

#include "Core/Base/Application.h"
#include "Core/Renderer/RenderContext.h"
#include "Core/Base/Window.h"
#include "Utilities/BitFlagsHelper.h"
#include "DebugLogHelper.h"

using namespace ByteEngine;

int32 Application::Run()
{
    Application::SetInstance(this);

    Window gameWindow;
    gameWindow.Initialize(L"ByteEngine D3D11 Application");
   
    RenderingContext renderManager;
    renderManager.Initialize(&gameWindow);
    RenderingContext::SetInstance(&renderManager);

    while (isRunning)
    {
        WindowEvents events = gameWindow.PollEvents();

        if (HasFlags(events, WindowEvents::CLOSE))
            break;

        renderManager.OnWindowModeChanged(events);
        renderManager.OnResize(events);

        renderManager.OnUpdate();
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
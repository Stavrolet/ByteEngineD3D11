#include <d3d11_1.h>
#include <Windows.h>

#include "Application.h"
#include "RenderManager.h"
#include "Window.h"
#include "EnumFlagsHelpers.h"
#include "DebugLogHelper.h"

using namespace ByteEngine;

int32 Application::Run()
{
    Application::SetInstance(this);

    Window gameWindow;
    gameWindow.Initialize(L"ByteEngine D3D11 Application");
   
    RenderManager renderManager;
    renderManager.Initialize(&gameWindow);
    RenderManager::SetInstance(&renderManager);

    while (isRunning)
    {
        WindowEvents events = gameWindow.PollEvents();

        if (HasFlags(events, WindowEvents::CLOSE))
            break;

        renderManager.OnFullScreenStateChanged(events);
        renderManager.OnResize(events);

        gameWindow.SetPreviousPlacement();

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
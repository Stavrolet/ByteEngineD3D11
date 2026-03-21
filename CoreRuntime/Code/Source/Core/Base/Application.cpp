#define WIN32_LEAN_AND_MEAN
#include <d3d11_1.h>
#include <Windows.h>

#include "ByteEngine/Core/Base/Application.h"
#include "ByteEngine/Core/Base/MainWindow.h"
#include "ByteEngine/Core/Input/Input.h"
#include "ByteEngine/Core/Renderer/RenderContext.h"
#include "ByteEngine/Utilities/BitFlagsHelper.h"
#include "ByteEngine/DebugLogHelper.h"

namespace ByteEngine
{
    void Application::Quit(int32 exitCode)
    {
        this->exitCode = exitCode;
        isRunning = false;
    }

    int32 Application::Run(MainWindow& mainWindow)
    {
        Application::SetInstance(this);

        RenderingContext renderingContext;
        renderingContext.Initialize();
        RenderingContext::SetInstance(&renderingContext);

        Input input;
        Input::SetInstance(&input);

        while (isRunning)
        {
            float backGroundColor[4] = { 0.0f, 0.1f, 0.15f, 1.0f };

            mainWindow.PollEvents();

            if (mainWindow.closeRequested)
            {
                if (quitRequest.HasSubscriber())
                {
                    if (quitRequest.Invoke())
                    {
                        DebugHelper::LogDebugMessage("Quit request was approved. Closing application.");
                        mainWindow.Close();
                        break;
                    }
                    else
                    {
                        DebugHelper::LogDebugMessage("Quit request was denied. Continuing application execution.");
                        mainWindow.closeRequested = false;
                    }
                }
                else
                {
                    DebugHelper::LogDebugMessage("Quit request was approved. Closing application.");
                    mainWindow.Close();
                    break;
                }
            }

            input.Update();

            if (Input::GetInstance().IsActionJustPressed("test1"))
            {
                backGroundColor[0] = 1.0f;
                backGroundColor[1] = 0.4f;
                backGroundColor[2] = 0.0f;
                backGroundColor[3] = 1.0f;
            }

            renderingContext.Update(backGroundColor);
        }

        DebugHelper::LogDebugMessage("Application is closing");

        return exitCode;
    }
}
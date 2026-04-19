#define WIN32_LEAN_AND_MEAN
#include <d3d11_1.h>
#include <Windows.h>

#include "ByteEngine/Core/Base/Application.h"
#include "ByteEngine/Core/Base/MainWindow.h"
#include "ByteEngine/Core/Input/Input.h"
#include "ByteEngine/Core/Renderer/RenderContext.h"
#include "ByteEngine/Utilities/BitFlagsHelper.h"
#include "ByteEngine/DebugLogHelper.h"
#include "Platform/Core/Graphics/GraphicsDeviceD3D11.h"

using namespace ByteEngine::Graphics;

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

        GraphicsDeviceD3D11 graphicsDeviceD3D11;

#ifdef _DEBUG
        GraphicsDevice::Error error = graphicsDeviceD3D11.Initialize(true);
#else
        GraphicsDevice::Error error = graphicsDeviceD3D11.Initialize(false);
#endif

        if (error != GraphicsDevice::Error::Success)
        {
            DebugHelper::LogDebugMessage("Failed to initialize graphics device. Error code: " + std::to_string(static_cast<int>(error)));
            return -1;
        }

        GraphicsDevice::SetInstance(&graphicsDeviceD3D11);

        Input input;
        Input::SetInstance(&input);

        while (isRunning)
        {
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
        }

        DebugHelper::LogDebugMessage("Application is closing");

        return exitCode;
    }
}
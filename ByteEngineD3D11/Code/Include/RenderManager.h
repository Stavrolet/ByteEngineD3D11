#pragma once

#include "Base/Singleton.h"
#include "TypesAliases/ComTypesAlisases.h"
#include "WindowEvents.h"

using namespace wil;

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace ByteEngine
{
    class Window;

    class RenderManager : public Singleton<RenderManager>
    {
        friend class Application;

    private:
        ComPtr<ID3D11Device1> device;
        ComPtr<ID3D11DeviceContext1> deviceContext;

        ComPtr<IDXGISwapChain1> swapChain;
        ComPtr<ID3D11RenderTargetView> renderTargetView;
        ComPtr<ID3D11DepthStencilView> depthStencilView;

        Window* targetWindow = nullptr;

    public:
        RenderManager() = default;
        virtual ~RenderManager() override; 

        void Initialize(Window* targetWindow);

    private:
        void CreateDeviceAndContext();
        void CreateDebugLayer();
        void CreateSwapChain();
        bool CreateRenderTargetAndDepthStencil();

        void OnUpdate();
        void OnResize(WindowEvents resizeEvent);
        void OnFullScreenStateChanged(WindowEvents fullScreenStateChanged);

        void ResizeSwapchain();
        void Reinitialize()
        {
            Cleanup();
            Initialize(targetWindow);
        }
        void Cleanup();
    };
}
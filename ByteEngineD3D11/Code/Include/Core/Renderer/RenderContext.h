#pragma once

#include <wil/com.h>

#include "Core/Base/Singleton.h"
#include "Core/Base/WindowEvents.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace ByteEngine
{
    template <typename T>
    using ComPtr = ::wil::com_ptr_nothrow<T>;

    class Window;

    class RenderingContext : public Singleton<RenderingContext>
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
        RenderingContext() = default;
        virtual ~RenderingContext() override; 

        void Initialize(Window* targetWindow);

    private:
        void CreateDeviceAndContext();
        void CreateDebugLayer();
        void CreateSwapChain();
        bool CreateRenderTargetAndDepthStencil();

        void OnUpdate();
        void OnResize(WindowEvents resizeEvent);
        void OnWindowModeChanged(WindowEvents fullScreenStateChanged);

        void ResizeSwapchain();
        void SetSwapChainFullscreenState(bool fullscreen);
        void Reinitialize()
        {
            Cleanup();
            Initialize(targetWindow);
        }
        void Cleanup();
    };
}
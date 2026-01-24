#pragma once

#include <wil/com.h>

#include "Core/Base/Singleton.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace ByteEngine
{
    struct WindowResizeEvent;
    struct WindowModeChangeEvent;

    class Window;

    class RenderingContext : public Singleton<RenderingContext>
    {
        friend class Application;

    private:
        wil::com_ptr_nothrow<ID3D11Device1> device;
        wil::com_ptr_nothrow<ID3D11DeviceContext1> deviceContext;

        wil::com_ptr_nothrow<IDXGISwapChain1> swapChain;
        wil::com_ptr_nothrow<ID3D11RenderTargetView> renderTargetView;
        wil::com_ptr_nothrow<ID3D11DepthStencilView> depthStencilView;

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
        void OnResize(const WindowResizeEvent* event);
        void OnWindowModeChanged(const WindowModeChangeEvent* event);

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
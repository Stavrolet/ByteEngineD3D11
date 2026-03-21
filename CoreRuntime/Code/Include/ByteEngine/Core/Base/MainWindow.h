#pragma once

#include <string>

#include "ByteEngine/CoreDefs.h"
#include "ByteEngine/Core/Base/Singleton.h"
#include "ByteEngine/Core/EventSystem/Delegate.h"
#include "ByteEngine/Core/EventSystem/MulticastDelegate.h"
#include "ByteEngine/Core/Input/KeyCode.h"
#include "ByteEngine/Math/Vector2i.h"
#include "ByteEngine/Primitives.h"

namespace ByteEngine
{
    using namespace EventSystem;

    enum class WindowMode : uint8
    {
        Minimized,
        Windowed,
        Mazimized,
        BorderlessFullscreen,
        ExclusiveFullscreen
    };

    class MainWindow;
    template class BYTEENGINE_API Singleton<MainWindow>;

    class MainWindow : public Singleton<MainWindow>
    {
        friend class Application;
        friend class Input;

    protected:
        void* handle = nullptr;

        ByteEngine::Math::Vector2i size;
        ByteEngine::Math::Vector2i position;

        bool initialized = false;

        WindowMode mode = WindowMode::BorderlessFullscreen;
        WindowMode previousMode = mode;

        bool hasFocus = true;

        bool closeRequested = false;
        bool destroyed = false;

        std::string title;

        DelegateVoid<KeyCode, bool> keyStateChanged;
        DelegateVoid<ByteEngine::Math::Vector2i> mouseMoved;
        DelegateVoid<float, float> mouseWheelStateChanged;

        MulticastDelegate<ByteEngine::Math::Vector2i> resized;

        MulticastDelegate<bool> focusStateChanged;

        MulticastDelegate<WindowMode> modeChanged;
        MulticastDelegate<std::string_view> titleChanged;

    public:
        MulticastDelegate<ByteEngine::Math::Vector2i>& Resized() { return resized; }
        MulticastDelegate<bool>& FocusStateChanged() { return focusStateChanged; }
        MulticastDelegate<WindowMode>& ModeChanged() { return modeChanged; }
        MulticastDelegate<std::string_view>& TitleChanged() { return titleChanged; }

        virtual ~MainWindow() = default;

        virtual void SetWindowMode(WindowMode modeToSet) = 0;
        virtual void SetWindowTitle(std::string title) = 0;

        virtual void SetWindowSize(::ByteEngine::Math::Vector2i size) { SetWindowSize(size.width, size.height); }
        virtual void SetWindowSize(int32 width, int32 height) = 0;

        virtual void SetWindowPosition(::ByteEngine::Math::Vector2i position) { SetWindowPosition(position.x, position.y); }
        virtual void SetWindowPosition(int32 x, int32 y) = 0;

        virtual void SetFocus() = 0;

        void* GetNativeHandle() const { return handle; }
        
        WindowMode GetMode() const { return mode; }
        const std::string& GetTitle() const { return title; }

        ByteEngine::Math::Vector2i GetSize() const { return size; }
        ByteEngine::Math::Vector2i GetPosition() const { return position; }

        bool HasFocus() const { return hasFocus; }

    protected:
        virtual void Close() = 0;
        virtual void PollEvents() = 0;

    private:
        DelegateVoid<KeyCode, bool>& KeyStateChanged() { return keyStateChanged; }
        DelegateVoid<ByteEngine::Math::Vector2i>& MouseMoved() { return mouseMoved; }
        DelegateVoid<float, float>& MouseWheelStateChanged() { return mouseWheelStateChanged; }
    };
}
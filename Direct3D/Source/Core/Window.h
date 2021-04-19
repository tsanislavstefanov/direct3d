#pragma once

#include "Core.h"
#include "Bindings/Event.h"
#include "Graphics/GraphicsContext.h"
#include "Math/Vector.h"

////////////////////////////////
// WINDOW PROPS ////////////////
////////////////////////////////

struct WindowProps
{
    uint32_t Width = 800, Height = 600;
    std::wstring Title = L"New Window";
    bool VSync = false;
};

////////////////////////////////
// MOUSE MOVED EVENT ///////////
////////////////////////////////

struct MouseMovedEvent
{
    MouseMovedEvent(const Vec2& position)
        :
        Position(position)
    {}

    const Vec2 Position;
};

////////////////////////////////
// MOUSE WHEEL MOVED EVENT /////
////////////////////////////////

struct MouseWheelMovedEvent
{
    MouseWheelMovedEvent(const Vec2& offset)
        :
        Offset(offset)
    {}

    const Vec2 Offset;
};

////////////////////////////////
// MOUSE BUTTON EVENT //////////
////////////////////////////////

struct MouseButtonEvent
{
    MouseButtonEvent(uint32_t button_code)
        :
        ButtonCode(button_code)
    {}
    virtual ~MouseButtonEvent() = default;

    const uint32_t ButtonCode;
};

////////////////////////////////
// MOUSE BUTTON DOWN EVENT /////
////////////////////////////////

struct MouseButtonDownEvent : public MouseButtonEvent
{
    MouseButtonDownEvent(uint32_t button_code)
        :
        MouseButtonEvent(button_code)
    {}
};

////////////////////////////////
// MOUSE BUTTON UP EVENT ///////
////////////////////////////////

struct MouseButtonUpEvent : public MouseButtonEvent
{
    MouseButtonUpEvent(uint32_t button_code)
        :
        MouseButtonEvent(button_code)
    {}
};

////////////////////////////////
// KEY EVENT ///////////////////
////////////////////////////////

struct KeyEvent
{
    KeyEvent(uint32_t key_code)
        :
        KeyCode(key_code)
    {}
    virtual ~KeyEvent() = default;

    const uint32_t KeyCode;
};

////////////////////////////////
// KEY DOWN EVENT //////////////
////////////////////////////////

struct KeyDownEvent : public KeyEvent
{
    KeyDownEvent(uint32_t key_code, uint32_t repeat_count)
        :
        KeyEvent(key_code),
        RepeatCount(repeat_count)
    {}

    const uint32_t RepeatCount;
};

////////////////////////////////
// KEY UP EVENT ////////////////
////////////////////////////////

struct KeyUpEvent : public KeyEvent
{
    KeyUpEvent(uint32_t key_code)
        :
        KeyEvent(key_code)
    {}
};

////////////////////////////////
// WINDOW RESIZED EVENT ////////
////////////////////////////////

struct WindowResizedEvent
{
    WindowResizedEvent(uint32_t width, uint32_t height)
        :
        Width(width),
        Height(height)
    {}

    const uint32_t Width, Height;
};

////////////////////////////////
// WINDOW CLOSE EVENT //////////
////////////////////////////////

struct WindowCloseEvent // No event data.
{};

////////////////////////////////
// WINDOW //////////////////////
////////////////////////////////

class Window
{
public:
    static Window* Main;

    Window(const WindowProps& props);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    bool GetVSync() const;

    void SetVSync(bool vsync);

    void DispatchEvents();
    void Update();

    EventDispatcher Dispatcher = {};
private:
    struct WindowData
    {
        uint32_t Width = 0, Height = 0;
        bool VSync = false;
        EventDispatcher* Dispatcher = nullptr;
        Scope<GraphicsContext> GraphicsContext = nullptr;
    };

    WindowData m_Data = {};
    HINSTANCE m_Module = nullptr;
    HWND m_Handle = nullptr;
};
#pragma once

#include "Core.h"
#include "Window.h"
#include "Bindings/Event.h"

class Application
{
public:
    Application(const WindowProps& props);
    virtual ~Application() = default;

    void Run();
protected:
    virtual void OnUpdate() = 0;

    Window m_Window;
private:
    void OnMouseButtonDown(MouseButtonDownEvent& e);
    void OnMouseMoved(MouseMovedEvent& e);
    void OnKeyDown(KeyDownEvent& e);
    void OnKeyUp(KeyUpEvent& e);
    void OnWindowResized(WindowResizedEvent& e);
    void OnWindowClose(WindowCloseEvent& e);

    bool m_Shutdown = false, m_Minimized = false;
};

Application* CreateApplication();
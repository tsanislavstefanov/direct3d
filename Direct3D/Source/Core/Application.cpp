#include "stdafx.h"
#include "Application.h"
#include "Input.h"
#include "Log.h"
#include "Time.h"
#include "Diagnostics/Stopwatch.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"

Application::Application(const WindowProps& props)
    :
    m_Window(props)
{
    m_Window.Dispatcher.Subscribe<MouseButtonDownEvent>().Connect<&Application::OnMouseButtonDown>(this);
    m_Window.Dispatcher.Subscribe<MouseMovedEvent>().Connect<&Application::OnMouseMoved>(this);
    m_Window.Dispatcher.Subscribe<KeyDownEvent>().Connect<&Application::OnKeyDown>(this);
    m_Window.Dispatcher.Subscribe<KeyUpEvent>().Connect<&Application::OnKeyUp>(this);
    m_Window.Dispatcher.Subscribe<WindowResizedEvent>().Connect<&Application::OnWindowResized>(this);
    m_Window.Dispatcher.Subscribe<WindowCloseEvent>().Connect<&Application::OnWindowClose>(this);
}

void Application::Run()
{
    Stopwatch timewatch = {}, framewatch = {};
    timewatch.Now(), framewatch.Now();

    while (!m_Shutdown)
    {
        const float seconds = timewatch.GetSeconds();
        Time::s_Seconds = seconds * Time::Scale;
        Time::s_FixedSeconds = Time::FixedTime * Time::Scale;
        timewatch.Now();

        m_Window.DispatchEvents();

        if (m_Minimized)
            continue; // Don't update application when minimized.

        Renderer::SetClearColor(Color::Black);
        Renderer::Clear();

        OnUpdate();

        m_Window.Update();

        if (framewatch.GetMilliseconds() >= 1000.0f)
        {
            const Renderer2DStats& stats = Renderer2D::GetStats();
            LOG_INFO("Quads: {0} ... draw calls: {1}", stats.QuadCount, stats.DrawCallCount);
            Renderer2D::ResetStats();

            Time::s_FrameCount = 0;
            framewatch.Now();
        }

        Time::s_FrameCount++;
    }
}

void Application::OnMouseButtonDown(MouseButtonDownEvent& e)
{
    Input::s_MouseButtonStates[e.ButtonCode] = true;
}

void Application::OnMouseMoved(MouseMovedEvent& e)
{
    Input::s_MousePosition = e.Position;
}

void Application::OnKeyDown(KeyDownEvent& e)
{
    Input::s_KeyStates[e.KeyCode] = true;

    if (e.KeyCode == KeyCode::LeftArrow)
        Input::s_AxisRaw.X = -1.0f;
    if (e.KeyCode == KeyCode::RightArrow)
        Input::s_AxisRaw.Y =  1.0f;
    if (e.KeyCode == KeyCode::UpArrow)
        Input::s_AxisRaw.W =  1.0f;
    if (e.KeyCode == KeyCode::DownArrow)
        Input::s_AxisRaw.Z = -1.0f;
}

void Application::OnKeyUp(KeyUpEvent& e)
{
    Input::s_KeyStates[e.KeyCode] = false;

    if (e.KeyCode == KeyCode::LeftArrow)
        Input::s_AxisRaw.X = 0.0f;
    if (e.KeyCode == KeyCode::RightArrow)
        Input::s_AxisRaw.Y = 0.0f;
    if (e.KeyCode == KeyCode::UpArrow)
        Input::s_AxisRaw.W = 0.0f;
    if (e.KeyCode == KeyCode::DownArrow)
        Input::s_AxisRaw.Z = 0.0f;
}

void Application::OnWindowResized(WindowResizedEvent& e)
{
    if (e.Width == 0 && e.Height == 0)
        m_Minimized = true;

    m_Minimized = false;
    Renderer::SetViewport(0, 0, e.Width, e.Height);
}

void Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Shutdown = true;
}
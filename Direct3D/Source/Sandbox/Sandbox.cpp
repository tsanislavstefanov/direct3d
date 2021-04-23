#include "stdafx.h"
#include "Sandbox.h"
#include "Bindings/Event.h"
#include "Graphics/GUI.h"

Application* CreateApplication()
{
    return new Sandbox({ 800, 600, L"Sandbox" });
}

Sandbox::Sandbox(const WindowProps& props)
    :
    Application(props)
{
    m_Window.Dispatcher.Subscribe<WindowResizedEvent>().Connect([](WindowResizedEvent& e) {
        GUI::SetViewportSize(e.Width, e.Height);
    });

    GUI::SetViewportSize(props.Width, props.Height);
    GUI::SetFontSize(16);

    GUIStyle style = {};
    memset(&style, 0, sizeof(style));
    style.Padding = 8.0f;
    style.Colors[GUIColor::Text] = Color::White;
    style.Colors[GUIColor::Button] = Color::FromHex(0x1d1c39);
    // style.Colors[GUIColor::ButtonActive] = Color::from;
    style.Colors[GUIColor::ButtonHovered] = Color::FromHex(0xff22b1);
    GUI::SetStyle(style);
}

void Sandbox::OnUpdate()
{
    OnGUI();
}

void Sandbox::OnGUI()
{
    if (GUI::Button({ 100.0f }, "New Button"))
    {

    }
}
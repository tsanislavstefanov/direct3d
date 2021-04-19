#include "stdafx.h"
#include "GUI.h"
#include "Camera.h"
#include "Color.h"
#include "Renderer2D.h"
#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/Window.h"

GUIContext GUI::s_Context = {};
GUIStyle GUI::s_Style = {};

void GUI::SetStyle(const GUIStyle& style)
{
    s_Style = style;
}

void GUI::SetViewportSize(uint32_t width, uint32_t height)
{
    s_Context.ViewportWidth = width;
    s_Context.ViewportHeight = height;
}

void GUI::SetFontSize(uint32_t height)
{
    const Ref<Font>& font = Renderer2D::GetFont();
    s_Context.FontScale = height / (float)font->GetHeight();
}

bool GUI::Button(const Vec2& position, const std::string& text)
{
    const OrthoCamera camera(0.0f, (float)s_Context.ViewportWidth, 0.0f, (float)s_Context.ViewportHeight);
    Renderer2D::BeginScene(camera);

    const Ref<Font>& font = Renderer2D::GetFont();
    Vec2 size = Vec2::Zero;
    const float scale = s_Context.FontScale;
    for (const char c : text)
    {
        const CharInfo& info = font->GetCharInfo(c);
        size += info.Advance * scale;
    }

    size += s_Style.Padding * 2.0f;
    const float descent = font->GetDescent();
    size.Y += (font->GetAscent() - descent) * scale;

    const Vec2& mouse_position = Input::GetMousePosition();
    const Window& window = *Window::Main;
    Vec2 fixed_mouse_position = mouse_position;
    fixed_mouse_position.Y = window.GetHeight() - mouse_position.Y;

    const Vec2 bottom_left = position; // Bottom - left.
    const Vec2 top_right = position + size; // Top - right.
    Color button_color = s_Style.Colors[GUIColor::Button];
    bool result = false;

    if (fixed_mouse_position.X >= bottom_left.X && fixed_mouse_position.X <= top_right.X
        && fixed_mouse_position.Y >= bottom_left.Y && fixed_mouse_position.Y <= top_right.Y)
    {
        button_color = s_Style.Colors[GUIColor::ButtonActive];
        result = true;
    }

    Renderer2D::DrawQuad(position, size, button_color);
    Vec2 offset = position;
    offset.Y -= descent * scale;
    Renderer2D::DrawText(text, offset + s_Style.Padding, scale, s_Style.Colors[GUIColor::Text]);
    Renderer2D::EndScene();

    return result;
}
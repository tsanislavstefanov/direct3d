#include "stdafx.h"
#include "GUI.h"
#include "Camera.h"
#include "Color.h"
#include "Renderer2D.h"
#include "Core/Core.h"
#include "Core/Input.h"

////////////////////////////////
// GUI /////////////////////////
////////////////////////////////

GUIContext GUI::s_Context = {};
GUIStyle GUI::s_Style = {};

const GUIStyle& GUI::GetStyle()
{
    return s_Style;
}

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

    Vec2 mouse_position = Input::GetMousePosition();
    mouse_position.Y = s_Context.ViewportHeight - mouse_position.Y;
    Color color = s_Style.Colors[GUIColor::Button];
    bool is_clicked = false;
    if (IsPointInRect(mouse_position, { position, position + size }))
    {
        color = s_Style.Colors[GUIColor::ButtonHovered];
        if (Input::GetMouseButtonDown(MouseButtonCode::Left))
            is_clicked = true;
    }

    const OrthoCamera camera(0.0f, (float)s_Context.ViewportWidth, 0.0f, (float)s_Context.ViewportHeight);
    Renderer2D::BeginScene(camera);
    Renderer2D::DrawQuad(position, size, color);
    Vec2 offset = position;
    offset.Y -= descent * scale;
    Renderer2D::DrawText(text, offset + s_Style.Padding, scale, s_Style.Colors[GUIColor::Text]);
    Renderer2D::EndScene();

    return is_clicked;
}

bool GUI::IsPointInRect(const Vec2& point, const Rect& rect)
{
    return point.X > rect.BottomLeft.X && point.X < rect.TopRight.X
        && point.Y > rect.BottomLeft.Y && point.Y < rect.TopRight.Y;
}
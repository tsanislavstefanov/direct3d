#pragma once

#include "Color.h"
#include "Math/Vector.h"

////////////////////////////////
// GUI CONTEXT /////////////////
////////////////////////////////

struct GUIContext
{
    uint32_t ViewportWidth = 0, ViewportHeight = 0;
    float FontScale = 16.0f / 64.0f;
    uint32_t HotItemId = 0, ActiveItemId = 0;
};

////////////////////////////////
// GUI COLOR(S) ////////////////
////////////////////////////////

enum GUIColor : size_t
{
    Text = 0,

    Button,
    ButtonActive,
    ButtonHovered,

    Count
};

////////////////////////////////
// GUI STYLE ///////////////////
////////////////////////////////

struct GUIStyle
{
    float Padding = 8.0f;
    Color Colors[GUIColor::Count];
};

////////////////////////////////
// GUI /////////////////////////
////////////////////////////////

class GUI
{
public:
    static void SetStyle(const GUIStyle& style);
    static void SetViewportSize(uint32_t width, uint32_t height);
    static void SetFontSize(uint32_t height);

    static bool Button(const Vec2& position, const std::string& text);
private:
    static GUIContext s_Context;
    static GUIStyle s_Style;
};
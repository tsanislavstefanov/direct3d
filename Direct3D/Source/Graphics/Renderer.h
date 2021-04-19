#pragma once

#include "Color.h"

////////////////////////////////
// FORWARD DECLARATION(S) //////
////////////////////////////////

class GraphicsContext;

////////////////////////////////
// RENDERER ////////////////////
////////////////////////////////

class Renderer
{
public:
    Renderer() = delete;
    Renderer(const Renderer&) = delete;

    Renderer& operator=(const Renderer&) = delete;

    static void SetClearColor(const Color& clear_color);
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    static void Init(const GraphicsContext& graphics_context);
    static void Clear();
    static void Shutdown();
private:
    static Color s_ClearColor;
    static ID3D11DeviceContext* s_DeviceContext;
    static ID3D11RenderTargetView* s_RenderTargetView;
};
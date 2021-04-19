#include "stdafx.h"
#include "Renderer.h"
#include "GraphicsContext.h"
#include "Renderer2D.h"

////////////////////////////////
// RENDERER ////////////////////
////////////////////////////////

Color Renderer::s_ClearColor = Color::White;
ID3D11DeviceContext* Renderer::s_DeviceContext = nullptr;
ID3D11RenderTargetView* Renderer::s_RenderTargetView = nullptr;

void Renderer::SetClearColor(const Color& clear_color)
{
    s_ClearColor = clear_color;
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    D3D11_VIEWPORT viewport;
    memset(&viewport, 0, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = (FLOAT)x;
    viewport.TopLeftY = (FLOAT)y;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    s_DeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::Init(const GraphicsContext& graphics_context)
{
    s_DeviceContext = graphics_context.GetDeviceContext();
    s_RenderTargetView = graphics_context.GetRenderTargetView();

    Renderer2D::Init(graphics_context);
}

void Renderer::Clear()
{
    s_DeviceContext->ClearRenderTargetView(s_RenderTargetView, (float*)&s_ClearColor);
}

void Renderer::Shutdown()
{
    Renderer2D::Shutdown();
}
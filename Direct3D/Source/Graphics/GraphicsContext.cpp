#include "stdafx.h"
#include "GraphicsContext.h"
#include "Renderer.h"

#pragma comment(lib, "d3d11.lib")

GraphicsContext::GraphicsContext(HWND hwnd, uint32_t width, uint32_t height)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = D3D11_CREATE_DEVICE_DEBUG;
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &m_SwapChain,
        &m_Device,
        nullptr,
        &m_DeviceContext
    );

    wrl::ComPtr<ID3D11Texture2D> buffer = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
    m_Device->CreateRenderTargetView(buffer.Get(), nullptr, &m_RenderTargetView);
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);

    Renderer::Init(*this);
    Renderer::SetViewport(0, 0, width, height);
}

GraphicsContext::~GraphicsContext()
{
    Renderer::Shutdown();
}

ID3D11Device* GraphicsContext::GetDevice() const
{
    return m_Device.Get();
}

ID3D11DeviceContext* GraphicsContext::GetDeviceContext() const
{
    return m_DeviceContext.Get();
}

ID3D11RenderTargetView* GraphicsContext::GetRenderTargetView() const
{
    return m_RenderTargetView.Get();
}

void GraphicsContext::SwapBuffers()
{
    m_SwapChain->Present(1, 0);
}

void GraphicsContext::ResizeBuffers()
{
    // Unbind render target view(s).
    m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_RenderTargetView = nullptr;

    // Resize swap chain's buffers.
    m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    // Create render target view(s) from swap chain's back buffer.
    wrl::ComPtr<ID3D11Texture2D> buffer = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
    m_Device->CreateRenderTargetView(buffer.Get(), nullptr, &m_RenderTargetView);

    // Bind render target view(s).
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}
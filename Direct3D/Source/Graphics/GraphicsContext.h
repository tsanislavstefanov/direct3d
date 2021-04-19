#pragma once

class GraphicsContext
{
public:
    GraphicsContext(HWND hwnd, uint32_t width, uint32_t height);
    ~GraphicsContext();

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;
    ID3D11RenderTargetView* GetRenderTargetView() const;

    void SwapBuffers();
    void ResizeBuffers();
private:
    wrl::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
    wrl::ComPtr<ID3D11Device> m_Device = nullptr;
    wrl::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
    wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView = nullptr;
};
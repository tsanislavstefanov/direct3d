#pragma once

class Blender
{
public:
    Blender(ID3D11Device* device, bool enable = true);

    void Bind(ID3D11DeviceContext* device_context);
private:
    wrl::ComPtr<ID3D11BlendState> m_BlendState = nullptr;
};
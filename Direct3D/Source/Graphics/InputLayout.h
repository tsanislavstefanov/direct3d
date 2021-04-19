#pragma once

#include "Buffer.h"

class InputLayout
{
public:
    InputLayout(ID3D11Device* device, ID3DBlob* blob, const VertexBufferLayout& layout);

    void Bind(ID3D11DeviceContext* device_context);
private:
    wrl::ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;
};
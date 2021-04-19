#include "stdafx.h"
#include "InputLayout.h"

InputLayout::InputLayout(ID3D11Device* device, ID3DBlob* blob, const VertexBufferLayout& layout)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {};
    for (const VertexAttrib& attrib : layout)
    {
        const std::string& name = attrib.GetName();
        const auto type = (DXGI_FORMAT)attrib.GetType();
        ied.push_back({ &name[0], 0, type, 0, attrib.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    const LPVOID bytecode = blob->GetBufferPointer();
    const SIZE_T size = blob->GetBufferSize();
    device->CreateInputLayout(&ied[0], ied.size(), bytecode, size, &m_InputLayout);
}

void InputLayout::Bind(ID3D11DeviceContext* device_context)
{
    device_context->IASetInputLayout(m_InputLayout.Get());
}
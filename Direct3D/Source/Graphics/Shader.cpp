#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"

#pragma comment(lib, "d3dcompiler.lib")

////////////////////////////////
// VERTEX SHADER ///////////////
////////////////////////////////

VertexShader::VertexShader(ID3D11Device* device, const std::wstring& filename)
{
    D3DReadFileToBlob(&filename[0], &m_Blob);
    const LPVOID bytecode = m_Blob->GetBufferPointer();
    const SIZE_T size = m_Blob->GetBufferSize();
    device->CreateVertexShader(bytecode, size, nullptr, &m_VertexShader);
}

ID3DBlob* VertexShader::GetBlob() const
{
    return m_Blob.Get();
}

void VertexShader::SetConstBuffer(ID3D11DeviceContext* device_context, const Ref<ConstBuffer>& cb, uint32_t slot)
{
    ID3D11Buffer* buffer = cb->GetBuffer();
    device_context->VSSetConstantBuffers(slot, 1, &buffer);
}

void VertexShader::Bind(ID3D11DeviceContext* device_context)
{
    device_context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
}

////////////////////////////////
// PIXEL SHADER ////////////////
////////////////////////////////

PixelShader::PixelShader(ID3D11Device* device, const std::wstring& filename)
{
    D3DReadFileToBlob(&filename[0], &m_Blob);
    const LPVOID bytecode = m_Blob->GetBufferPointer();
    const SIZE_T size = m_Blob->GetBufferSize();
    device->CreatePixelShader(bytecode, size, nullptr, &m_PixelShader);
}

void PixelShader::SetConstBuffer(ID3D11DeviceContext* device_context, const Ref<ConstBuffer>& cb, uint32_t slot)
{
    ID3D11Buffer* buffer = cb->GetBuffer();
    device_context->PSSetConstantBuffers(slot, 1, &buffer);
}

void PixelShader::Bind(ID3D11DeviceContext* device_context)
{
    device_context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}
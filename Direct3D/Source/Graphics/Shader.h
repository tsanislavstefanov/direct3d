#pragma once

#include "Core/Core.h"

////////////////////////////////
// FORWARD DECLARATION(S) //////
////////////////////////////////

class ConstBuffer;

////////////////////////////////
// VERTEX SHADER ///////////////
////////////////////////////////

class VertexShader
{
public:
    VertexShader(ID3D11Device* device, const std::wstring& filename);

    ID3DBlob* GetBlob() const;

    void SetConstBuffer(ID3D11DeviceContext* device_context, const Ref<ConstBuffer>& cb, uint32_t slot = 0);

    void Bind(ID3D11DeviceContext* device_context);
private:
    wrl::ComPtr<ID3DBlob> m_Blob = nullptr;
    wrl::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
};

////////////////////////////////
// PIXEL SHADER ////////////////
////////////////////////////////

class PixelShader
{
public:
    PixelShader(ID3D11Device* device, const std::wstring& filename);

    void SetConstBuffer(ID3D11DeviceContext* device_context, const Ref<ConstBuffer>& cb, uint32_t slot = 0);

    void Bind(ID3D11DeviceContext* device_context);
private:
    wrl::ComPtr<ID3DBlob> m_Blob = nullptr;
    wrl::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
};
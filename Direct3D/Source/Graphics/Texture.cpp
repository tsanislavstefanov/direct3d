#include "stdafx.h"

#include <stb/stb_image.h>

#include "Texture.h"

////////////////////////////////
// TEXTURE /////////////////////
////////////////////////////////

static DXGI_FORMAT GetFormatFromChannels(uint32_t channels)
{
    switch (channels)
    {
        case 1: return DXGI_FORMAT_R8_UNORM;
        case 2: return DXGI_FORMAT_R8G8_UNORM;
        case 4: return DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    return DXGI_FORMAT_UNKNOWN;
}

Texture2D::Texture2D(ID3D11Device* device, const std::string& filename)
{
    uint8_t* pixels = stbi_load(&filename[0], &m_Width, &m_Height, &m_Channels, 4);

    D3D11_TEXTURE2D_DESC t2dd = {};
    memset(&t2dd, 0, sizeof(D3D11_TEXTURE2D_DESC));
    t2dd.Width = m_Width;
    t2dd.Height = m_Height;
    t2dd.MipLevels = 1;
    t2dd.ArraySize = 1;
    t2dd.Format = GetFormatFromChannels(m_Channels);
    t2dd.SampleDesc.Count = 1;

    UINT quality = 0;
    device->CheckMultisampleQualityLevels(t2dd.Format, t2dd.SampleDesc.Count, &quality);
    t2dd.SampleDesc.Quality = quality - 1;

    t2dd.Usage = D3D11_USAGE_DEFAULT;
    t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SUBRESOURCE_DATA srd = {};
    memset(&srd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    srd.pSysMem = pixels;
    srd.SysMemPitch = m_Width * sizeof(uint8_t) * m_Channels;
    wrl::ComPtr<ID3D11Texture2D> texture2d = nullptr;
    device->CreateTexture2D(&t2dd, &srd, &texture2d);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
    memset(&srvd, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvd.Format = t2dd.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(texture2d.Get(), &srvd, &m_ShaderResourceView);

    stbi_image_free(pixels);
}

Texture2D::Texture2D(ID3D11Device* device, uint32_t width, uint32_t height, uint8_t* pixels, uint32_t channels)
    :
    m_Width(width),
    m_Height(height),
    m_Channels(channels)
{
    D3D11_TEXTURE2D_DESC t2dd = {};
    memset(&t2dd, 0, sizeof(D3D11_TEXTURE2D_DESC));
    t2dd.Width = m_Width;
    t2dd.Height = m_Height;
    t2dd.MipLevels = 1;
    t2dd.ArraySize = 1;
    t2dd.Format = GetFormatFromChannels(m_Channels);
    t2dd.SampleDesc.Count = 1;

    UINT quality = 0;
    device->CheckMultisampleQualityLevels(t2dd.Format, t2dd.SampleDesc.Count, &quality);
    t2dd.SampleDesc.Quality = quality - 1;

    t2dd.Usage = D3D11_USAGE_DEFAULT;
    t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SUBRESOURCE_DATA srd = {};
    memset(&srd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    srd.pSysMem = pixels;
    srd.SysMemPitch = m_Width * sizeof(uint8_t) * m_Channels;
    wrl::ComPtr<ID3D11Texture2D> texture2d = nullptr;
    device->CreateTexture2D(&t2dd, &srd, &texture2d);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
    memset(&srvd, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvd.Format = t2dd.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(texture2d.Get(), &srvd, &m_ShaderResourceView);
}

bool Texture2D::operator==(const Texture2D& other)
{
    return m_ShaderResourceView.Get() == other.GetShaderResourceView();
}

bool Texture2D::operator!=(const Texture2D& other)
{
    return !((*this) == other);
}

uint32_t Texture2D::GetWidth() const
{
    return (uint32_t)m_Width;
}

uint32_t Texture2D::GetHeight() const
{
    return (uint32_t)m_Height;
}

uint32_t Texture2D::GetChannels() const
{
    return (uint32_t)m_Channels;
}

ID3D11ShaderResourceView* Texture2D::GetShaderResourceView() const
{
    return m_ShaderResourceView.Get();
}

void Texture2D::Bind(ID3D11DeviceContext* device_context, uint32_t slot)
{
    device_context->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
}

////////////////////////////////
// SUB - TEXTURE 2D ////////////
////////////////////////////////

Ref<SubTexture2D> SubTexture2D::CreateFromCoord(const Ref<Texture2D>& texture, const Vec2& coord, const Vec2& size)
{
    const uint32_t width = texture->GetWidth(), height = texture->GetHeight();
    const Vec2 min = { (coord.X * size.X) / width, (coord.Y * size.Y) / height };
    const Vec2 max = { ((1 + coord.X) * size.X) / width, ((1 + coord.Y) * size.Y) / height };
    return CreateRef<SubTexture2D>(texture, min, max);
}

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const Vec2& min, const Vec2& max)
    :
    m_Texture(texture)
{
    m_TexCoords[0] = { min.X, max.Y };
    m_TexCoords[1] = { min.X, min.Y };
    m_TexCoords[2] = { max.X, min.Y };
    m_TexCoords[3] = { max.X, max.Y };
}

const Ref<Texture2D>& SubTexture2D::GetTexture() const
{
    return m_Texture;
}

const Vec2* SubTexture2D::GetTexCoords() const
{
    return m_TexCoords;
}

////////////////////////////////
// SAMPLER /////////////////////
////////////////////////////////

Sampler::Sampler(ID3D11Device* device)
{
    D3D11_SAMPLER_DESC sd = {};
    memset(&sd, 0, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    device->CreateSamplerState(&sd, &m_SamplerState);
}

void Sampler::Bind(ID3D11DeviceContext* device_context, uint32_t slot)
{
    device_context->PSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
}
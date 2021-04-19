#include "stdafx.h"
#include "Buffer.h"

////////////////////////////////
// VERTEX ATTRIB ///////////////
////////////////////////////////

VertexAttrib::VertexAttrib(const std::string& name, VertexAttribType type)
    :
    m_Name(name),
    m_Type(type)
{}

const std::string& VertexAttrib::GetName() const
{
    return m_Name;
}

const VertexAttribType& VertexAttrib::GetType() const
{
    return m_Type;
}

size_t VertexAttrib::GetSize() const
{
    switch (m_Type)
    {
        case VertexAttribType::FLOAT : return (size_t)(sizeof(float));
        case VertexAttribType::FLOAT2: return (size_t)(sizeof(float) * 2);
        case VertexAttribType::FLOAT3: return (size_t)(sizeof(float) * 3);
        case VertexAttribType::FLOAT4: return (size_t)(sizeof(float) * 4);
    }
    return 0;
}

////////////////////////////////
// VERTEX BUFFER LAYOUT ////////
////////////////////////////////

VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexAttrib>& attribs)
    :
    m_Attribs(attribs)
{
    size_t offset = 0;
    for (VertexAttrib& attrib : m_Attribs)
    {
        attrib.Offset = offset;
        const size_t size = attrib.GetSize();
        offset += size;
        m_Stride += size;
    }
}

VertexBufferLayout::Iterator VertexBufferLayout::begin()
{
    return m_Attribs.begin();
}

VertexBufferLayout::Iterator VertexBufferLayout::end()
{
    return m_Attribs.end();
}

VertexBufferLayout::ConstIterator VertexBufferLayout::begin() const
{
    return m_Attribs.begin();
}

VertexBufferLayout::ConstIterator VertexBufferLayout::end() const
{
    return m_Attribs.end();
}

size_t VertexBufferLayout::GetStride() const
{
    return m_Stride;
}

const VertexBufferLayout::AttribList& VertexBufferLayout::GetAttribs() const
{
    return m_Attribs;
}

////////////////////////////////
// VERTEX BUFFER ///////////////
////////////////////////////////

VertexBuffer::VertexBuffer(ID3D11Device* device, const float* vertices, uint32_t count, const VertexBufferLayout& layout)
    :
    m_Layout(layout)
{
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_Layout.GetStride() * count;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA srd = {};
    memset(&srd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    srd.pSysMem = vertices;
    device->CreateBuffer(&bd, &srd, &m_Buffer);
}

VertexBuffer::VertexBuffer(ID3D11Device* device, uint32_t count, const VertexBufferLayout& layout)
    :
    m_Layout(layout)
{
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_Layout.GetStride() * count;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&bd, NULL, &m_Buffer);
}

const VertexBufferLayout& VertexBuffer::GetLayout() const
{
    return m_Layout;
}

void VertexBuffer::SetSubData(ID3D11DeviceContext* device_context, const float* vertices, uint32_t count)
{
    D3D11_MAPPED_SUBRESOURCE msr;
    memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
    device_context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    memcpy(msr.pData, vertices, m_Layout.GetStride() * count);
    device_context->Unmap(m_Buffer.Get(), 0);
}

void VertexBuffer::Bind(ID3D11DeviceContext* device_context)
{
    const uint32_t offset = 0;
    const uint32_t stride = m_Layout.GetStride();
    device_context->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &stride, &offset);
}

////////////////////////////////
// INDEX BUFFER ////////////////
////////////////////////////////

IndexBuffer::IndexBuffer(ID3D11Device* device, const uint32_t* indices, uint32_t count)
{
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(uint32_t) * count;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA srd;
    memset(&srd, 0, sizeof(D3D11_SUBRESOURCE_DATA));
    srd.pSysMem = indices;
    device->CreateBuffer(&bd, &srd, &m_Buffer);
}

void IndexBuffer::Bind(ID3D11DeviceContext* device_context)
{
    device_context->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

////////////////////////////////
// CONST. BUFFER ///////////////
////////////////////////////////

ConstBuffer::ConstBuffer(ID3D11Device* device, size_t size)
{
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = size;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&bd, NULL, &m_Buffer);
}

ID3D11Buffer* ConstBuffer::GetBuffer() const
{
    return m_Buffer.Get();
}

void ConstBuffer::SetSubData(ID3D11DeviceContext* device_context, const void* data, size_t size)
{
    D3D11_MAPPED_SUBRESOURCE msr;
    memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
    device_context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    memcpy(msr.pData, data, size);
    device_context->Unmap(m_Buffer.Get(), 0);
}
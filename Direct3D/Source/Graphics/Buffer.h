#pragma once

////////////////////////////////
// VERTEX ATTRIB. //////////////
////////////////////////////////

enum class VertexAttribType
{
    Unknown = 0,
    FLOAT = 41,
    FLOAT2 = 16,
    FLOAT3 = 6,
    FLOAT4 = 2
};

class VertexAttrib
{
public:
    VertexAttrib(const std::string& name, VertexAttribType type);

    const std::string& GetName() const;
    const VertexAttribType& GetType() const;
    size_t GetSize() const;

    size_t Offset = 0;
private:
    std::string m_Name = "";
    VertexAttribType m_Type = VertexAttribType::Unknown;
};

////////////////////////////////
// VERTEX BUFFER LAYOUT ////////
////////////////////////////////

class VertexBufferLayout
{
public:
    using AttribList = std::vector<VertexAttrib>;
    using Iterator = AttribList::iterator;
    using ConstIterator = AttribList::const_iterator;

    VertexBufferLayout(const std::initializer_list<VertexAttrib>& attribs);

    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;

    size_t GetStride() const;
    const AttribList& GetAttribs() const;
private:
    size_t m_Stride = 0;
    AttribList m_Attribs;
};

////////////////////////////////
// VERTEX BUFFER ///////////////
////////////////////////////////

class VertexBuffer
{
public:
    VertexBuffer(ID3D11Device* device, const float* vertices, uint32_t count, const VertexBufferLayout& layout);
    VertexBuffer(ID3D11Device* device, uint32_t count, const VertexBufferLayout& layout);

    const VertexBufferLayout& GetLayout() const;

    void SetSubData(ID3D11DeviceContext* device_context, const float* vertices, uint32_t count);

    void Bind(ID3D11DeviceContext* device_context);
private:
    VertexBufferLayout m_Layout;
    wrl::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
};

////////////////////////////////
// INDEX BUFFER ////////////////
////////////////////////////////

class IndexBuffer
{
public:
    IndexBuffer(ID3D11Device* device, const uint32_t* indices, uint32_t count);

    void Bind(ID3D11DeviceContext* device_context);
private:
    wrl::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
};

////////////////////////////////
// CONST. BUFFER ///////////////
////////////////////////////////

class ConstBuffer
{
public:
    ConstBuffer(ID3D11Device* device, size_t size);

    ID3D11Buffer* GetBuffer() const;

    void SetSubData(ID3D11DeviceContext* device_context, const void* data, size_t size);
protected:
    wrl::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
};
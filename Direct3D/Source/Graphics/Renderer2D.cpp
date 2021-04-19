#pragma warning (disable : 6262 28020 6385)

#include "stdafx.h"
#include "Blender.h"
#include "Renderer2D.h"
#include "Buffer.h"
#include "GraphicsContext.h"
#include "InputLayout.h"
#include "Shader.h"

////////////////////////////////
// VERTEX //////////////////////
////////////////////////////////

struct Vertex
{
    Vec4 Position = Vec4::Zero;
    Color Color = Color::White;
    Vec2 TexCoord = Vec2::Zero;
    float TexIndex = 0.0f;
};

////////////////////////////////
// RENDERER 2D DATA ////////////
////////////////////////////////

static const uint32_t MaxQuads = 10000;
static const uint32_t MaxQuadVertices = MaxQuads * 4;
static const uint32_t MaxQuadIndices = MaxQuads * 6;
static const uint32_t MaxTexSlots = 4;

struct Renderer2DData
{
    Vertex* Vertices = nullptr;
    Ref<VertexBuffer> VB = nullptr;
    uint32_t VertexCount = 0;

    Ref<ConstBuffer> CBViewProj = nullptr;

    Ref<VertexShader> VS = nullptr;
    Ref<InputLayout> IL = nullptr;

    uint32_t WhiteTexIndex = 0, FontAtlasIndex = 1;
    Ref<Texture2D> Textures[4] = { nullptr };
    const uint32_t DefaultTexIndex = 2;
    uint32_t TexIndex = DefaultTexIndex; // Next available texture index.
    Ref<Sampler> TexSampler = nullptr;
    Ref<Blender> TexBlender = nullptr;

    Ref<Font> Font = nullptr;

    Ref<PixelShader> PS = nullptr;

    Ref<IndexBuffer> IB = nullptr;
    uint32_t IndexCount = 0;

    Renderer2D::Stats Stats = {};
};

static Renderer2DData s_Data = {};

////////////////////////////////
// RENDERER 2D /////////////////
////////////////////////////////

ID3D11Device* Renderer2D::s_Device = nullptr;
ID3D11DeviceContext* Renderer2D::s_DeviceContext = nullptr;

const Ref<Font>& Renderer2D::GetFont()
{
    return s_Data.Font;
}

const Renderer2D::Stats& Renderer2D::GetStats()
{
    return s_Data.Stats;
}

void Renderer2D::SetFont(const std::string& filename)
{
    const Ref<Font> font = CreateRef<Font>(s_Device, filename);
    SetFont(font);
}

void Renderer2D::SetFont(const Ref<Font>& font)
{
    s_Data.Font = font;
    s_Data.Textures[s_Data.FontAtlasIndex] = s_Data.Font->GetAtlas();
}

void Renderer2D::Init(const GraphicsContext& graphics_context)
{
    s_Device = graphics_context.GetDevice();
    s_DeviceContext = graphics_context.GetDeviceContext();

    s_Data.Vertices = new Vertex[MaxQuadVertices];
    const VertexBufferLayout layout = {
        VertexAttrib("POSITION", VertexAttribType::FLOAT4),
        VertexAttrib("COLOR", VertexAttribType::FLOAT4),
        VertexAttrib("TEX_COORD", VertexAttribType::FLOAT2),
        VertexAttrib("TEX_INDEX", VertexAttribType::FLOAT)
    };
    s_Data.VB = CreateRef<VertexBuffer>(s_Device, MaxQuadVertices, layout);
    s_Data.VertexCount = 0;

    s_Data.VS = CreateRef<VertexShader>(s_Device, L"Shaders/DefaultQuad.vertex.cso");
    ID3DBlob* blob = s_Data.VS->GetBlob();
    s_Data.IL = CreateRef<InputLayout>(s_Device, blob, layout);

    s_Data.CBViewProj = CreateRef<ConstBuffer>(s_Device, sizeof(Mat4));

    const uint32_t color = 0xffffffff;
    s_Data.Textures[s_Data.WhiteTexIndex] = CreateRef<Texture2D>(s_Device, 1, 1, (uint8_t*)&color);
    for (size_t i = s_Data.TexIndex; i < MaxTexSlots; i++)
        s_Data.Textures[i] = nullptr;

    s_Data.Font = CreateRef<Font>(s_Device, "Fonts/Roboto.ttf");
    s_Data.Textures[s_Data.FontAtlasIndex] = s_Data.Font->GetAtlas();

    s_Data.TexSampler = CreateRef<Sampler>(s_Device);
    s_Data.TexBlender = CreateRef<Blender>(s_Device);
    s_Data.PS = CreateRef<PixelShader>(s_Device, L"Shaders/DefaultQuad.pixel.cso");

    uint32_t indices[MaxQuadIndices];
    uint32_t offset = 0;
    for (size_t i = 0; i < MaxQuadIndices; i += 6)
    {
        indices[i] = offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 3 + offset;
        indices[i + 4] = offset;
        indices[i + 5] = 2 + offset;

        offset += 4;
    }
    s_Data.IB = CreateRef<IndexBuffer>(s_Device, indices, MaxQuadIndices);
    s_Data.IndexCount = 0;
}

void Renderer2D::ResetStats()
{
    memset(&s_Data.Stats, 0, sizeof(Renderer2D::Stats));
}

void Renderer2D::BeginScene(const Camera& camera)
{
    s_Data.VS->Bind(s_DeviceContext);
    s_Data.IL->Bind(s_DeviceContext);
    s_Data.PS->Bind(s_DeviceContext);

    const Mat4 view_proj = Mat4::Transpose(camera.GetViewProj());
    s_Data.CBViewProj->SetSubData(s_DeviceContext, &view_proj, sizeof(Mat4));
    s_Data.VS->SetConstBuffer(s_DeviceContext, s_Data.CBViewProj);

    s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    BeginBatch();
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& scale, const Color& color)
{
    DrawQuad(s_Data.Textures[s_Data.WhiteTexIndex], position, scale, color);
}

void Renderer2D::DrawQuad(const Ref<Texture2D>& texture, const Vec2& position, const Vec2& scale, const Color& color)
{
    const Vec2 min(0.0f);
    const Vec2 max(1.0f);
    const Ref<SubTexture2D> subtexture = CreateRef<SubTexture2D>(texture, min, max);
    DrawQuad(subtexture, position, scale, color);
}

void Renderer2D::DrawQuad(const Ref<SubTexture2D>& subtexture, const Vec2& position, const Vec2& scale, const Color& color)
{
    Mat4 transformation = Mat4::Translate({ 1.0f }, { position, 0.0f });
    transformation = Mat4::Scale(transformation, { scale, 1.0f });
    DrawQuad(subtexture, transformation, color);
}

void Renderer2D::DrawQuad(const Ref<SubTexture2D>& subtexture, const Mat4& transformation, const Color& color)
{
    if (s_Data.VertexCount >= MaxQuadVertices || s_Data.IndexCount >= MaxQuadIndices)
        NextBatch();

    const Ref<Texture2D>& texture = subtexture->GetTexture();

    int tex_index = -1;
    for (size_t i = 0; i < MaxTexSlots; i++)
    {
        if (s_Data.Textures[i] == texture)
        {
            tex_index = (float)i;
            break;
        }
    }

    if (tex_index == -1)
    {
        if (s_Data.TexIndex >= MaxTexSlots)
            NextBatch();

        tex_index = (float)s_Data.TexIndex;
        s_Data.Textures[s_Data.TexIndex++] = texture;
    }

    const Vec2 positions[] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };
    const Vec2* tex_coords = subtexture->GetTexCoords();

    for (size_t i = 0; i < 4; i++)
    {
        s_Data.Vertices[s_Data.VertexCount].Position = transformation * Vec4({ positions[i], 0.0f }, 1.0f);
        s_Data.Vertices[s_Data.VertexCount].Color = color;
        s_Data.Vertices[s_Data.VertexCount].TexCoord = tex_coords[i];
        s_Data.Vertices[s_Data.VertexCount].TexIndex = (float)tex_index;
        s_Data.VertexCount++;
    }

    s_Data.IndexCount += 6;
    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawText(const std::string& text, const Vec2& position, uint32_t height, const Color& color)
{
    const float scale = height / (float)s_Data.Font->GetHeight();
    DrawText(text, position, scale, color);
}

void Renderer2D::DrawText(const std::string& text, const Vec2& position, float scale, const Color& color)
{
    Vec2 offset = position;
    for (const char c : text)
    {
        const CharInfo& info = s_Data.Font->GetCharInfo(c);
        DrawChar(info, s_Data.Textures[s_Data.FontAtlasIndex], offset, scale, color);
        offset += (info.Advance) * scale;
    }
}

void Renderer2D::DrawChar(char c, const Vec2& position, uint32_t height, const Color& color)
{
    const float scale = height / (float)s_Data.Font->GetHeight();
    const CharInfo& info = s_Data.Font->GetCharInfo(c);
    DrawChar(info, s_Data.Textures[s_Data.FontAtlasIndex], position, scale, color);
}

void Renderer2D::DrawChar(const CharInfo& info, const Ref<Texture2D>& atlas, const Vec2& position, float scale, const Color& color)
{
    const Ref<SubTexture2D> subtexture = CreateRef<SubTexture2D>(atlas, info.Min, info.Max);
    DrawQuad(subtexture, position + (info.Bearing * scale), info.Size * scale, color);
}

void Renderer2D::EndScene()
{
    EndBatch();
}

void Renderer2D::Shutdown()
{
    delete[] s_Data.Vertices;
}

void Renderer2D::BeginBatch()
{
    s_Data.VertexCount = 0;
    s_Data.IndexCount = 0;
    s_Data.TexIndex = s_Data.DefaultTexIndex;
}

void Renderer2D::EndBatch()
{
    if (s_Data.VertexCount <= 0)
        return; // No Quads to draw.

    s_Data.VB->SetSubData(s_DeviceContext, (float*)s_Data.Vertices, s_Data.VertexCount);
    s_Data.VB->Bind(s_DeviceContext);

    for (size_t i = 0; i < MaxTexSlots; i++)
    {
        const Ref<Texture2D> texture = s_Data.Textures[i];
        if (!texture)
            break;

        texture->Bind(s_DeviceContext, i);
    }

    s_Data.TexSampler->Bind(s_DeviceContext);
    s_Data.TexBlender->Bind(s_DeviceContext);
    s_Data.IB->Bind(s_DeviceContext);
    s_DeviceContext->DrawIndexed(s_Data.IndexCount, 0, 0);

    s_Data.Stats.DrawCallCount++;
}

void Renderer2D::NextBatch()
{
    EndBatch();
    BeginBatch();
}
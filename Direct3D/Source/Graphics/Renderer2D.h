#pragma once

#ifdef DrawText
    #undef DrawText
#endif

#include "Camera.h"
#include "Color.h"
#include "Font.h"
#include "Texture.h"
#include "Core/Core.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

////////////////////////////////
// FORWARD DECLARATION(S) //////
////////////////////////////////

class GraphicsContext;

////////////////////////////////
// STATS ///////////////////////
////////////////////////////////

struct Renderer2DStats
{
    uint32_t DrawCallCount = 0;
    uint32_t QuadCount = 0;
};

////////////////////////////////
// RENDERER 2D /////////////////
////////////////////////////////

class Renderer2D
{
public:
    Renderer2D() = delete;
    Renderer2D(const Renderer2D&) = delete;

    Renderer2D& operator=(const Renderer2D&) = delete;

    static const Ref<Font>& GetFont();
    static const Renderer2DStats& GetStats();

    static void SetFont(const std::string& filename);
    static void SetFont(const Ref<Font>& font);

    static void Init(const GraphicsContext& graphics_context);
    static void ResetStats();
    static void BeginScene(const Camera& camera);
    static void DrawQuad(const Vec2& position, const Vec2& scale = { 1.0f }, const Color& color = Color::White);
    static void DrawQuad(const Ref<Texture2D>& texture, const Vec2& position, const Vec2& scale = { 1.0f }, const Color& color = Color::White);
    static void DrawQuad(const Ref<SubTexture2D>& subtexture, const Vec2& position, const Vec2& scale = { 1.0f }, const Color& color = Color::White);
    static void DrawQuad(const Ref<SubTexture2D>& subtexture, const Mat4& transformation, const Color& color = Color::White);
    static void DrawText(const std::string& text, const Vec2& position, uint32_t height = 16, const Color& color = Color::White);
    static void DrawText(const std::string& text, const Vec2& position, float scale, const Color& color = Color::White);
    static void DrawChar(char c, const Vec2& position, uint32_t height = 16, const Color& color = Color::White);
    static void DrawChar(const CharInfo& info, const Ref<Texture2D>& atlas, const Vec2& position, float scale, const Color& color = Color::White);
    static void EndScene();
    static void Shutdown();
private:
    static void BeginBatch();
    static void EndBatch();
    static void NextBatch();

    static ID3D11Device* s_Device;
    static ID3D11DeviceContext* s_DeviceContext;
};
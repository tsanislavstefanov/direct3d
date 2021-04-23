#pragma once

#include <stb/stb_truetype.h>

#include "Texture.h"
#include "Core/Core.h"
#include "Math/Vector.h"

////////////////////////////////
// CHAR INFO ///////////////////
////////////////////////////////

struct CharInfo
{
    Vec2 Bearing = Vec2::Zero;
    Vec2 Size = Vec2::Zero;
    Vec2 Advance = Vec2::Zero;
    Vec2 Min = Vec2::Zero;
    Vec2 Max = Vec2::Zero;
};

////////////////////////////////
// FONT ////////////////////////
////////////////////////////////

class Font
{
public:
    static const char FirstChar = ' ', LastChar = '~';
    static const uint16_t CharCount = (uint16_t)(LastChar - FirstChar);

    Font(ID3D11Device* device, const std::string& filename, uint32_t height = 64);

    uint32_t GetHeight() const;
    int GetAscent() const;
    int GetDescent() const;
    const Ref<Texture2D>& GetAtlas() const;
    const CharInfo& GetCharInfo(char c) const;
private:
    std::vector<uint8_t> m_Bytes = {};
    uint32_t m_Height;
    float m_Scale = 0.0f;
    int m_Ascent = 0, m_Descent = 0;
    CharInfo m_CharsInfo[CharCount] = {};
    Ref<Texture2D> m_Atlas = nullptr;
};
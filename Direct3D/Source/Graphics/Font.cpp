#include "stdafx.h"

// #include <stb/stb_image_write.h>

#include "Font.h"

Font::Font(ID3D11Device* device, const std::string& filename, uint32_t height)
    :
    m_Height(height)
{
    std::ifstream file(&filename[0], std::ios::in | std::ios::binary);
    ASSERT(!file.is_open(), "Cannot open {0}!", filename);

    file.seekg(0, std::ios::end);
    const size_t size = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);

    m_Bytes.resize(size);
    file.read((char*)&m_Bytes[0], size);
    file.close();

    stbtt_fontinfo font;
    memset(&font, 0, sizeof(stbtt_fontinfo));
    ASSERT(!stbtt_InitFont(&font, &m_Bytes[0], 0), "{0} could not be initialized!", filename);

    m_Scale = stbtt_ScaleForPixelHeight(&font, m_Height);
    int line_gap;
    stbtt_GetFontVMetrics(&font, &m_Ascent, &m_Descent, &line_gap);
    m_Ascent = (int)(m_Ascent * m_Scale);
    m_Descent = (int)(m_Descent * m_Scale);
    const int baseline = m_Ascent - m_Descent;
    int y = baseline;
    line_gap = (int)(line_gap * m_Scale);
    const int line_height = baseline + line_gap;

    const int tex_size = (int)(sqrtf(CharCount) + 1.0f) * (int)height;
    std::vector<uint8_t> pixels(tex_size * tex_size);
    int x = 0;

    for (size_t i = 0; i < CharCount; i++)
    {
        const int codepoint = FirstChar + i;

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, codepoint, &advance, &lsb);
        advance = (int)(advance * m_Scale);
        lsb = (int)(lsb * m_Scale);

        int minx, miny, maxx, maxy;
        stbtt_GetCodepointBitmapBox(&font, codepoint, m_Scale, m_Scale, &minx, &miny, &maxx, &maxy);
        const int width = maxx - minx;
        const int height = maxy - miny;

        if (x + advance >= tex_size)
        {
            x = 0;
            y += baseline + line_gap;
        }

        const int offset = x + lsb + (y + miny) * tex_size;
        stbtt_MakeCodepointBitmap(&font, &pixels[0] + offset, width, height, tex_size, m_Scale, m_Scale, codepoint);

        m_CharsInfo[i].Bearing = Vec2((float)lsb, -(float)maxy);
        m_CharsInfo[i].Size = Vec2((float)width, (float)height);
        m_CharsInfo[i].Advance = Vec2((float)advance, 0.0f);
        m_CharsInfo[i].Min = Vec2((x + lsb) / (float)tex_size, (y + miny) / (float)tex_size);
        m_CharsInfo[i].Max = Vec2((x + lsb + width) / (float)tex_size, (y + miny + height) / (float)tex_size);

        x += advance;
    }

    // stbi_write_png("FontAtlas.png", tex_size, tex_size, 1, &pixels[0], tex_size);

    m_Atlas = CreateRef<Texture2D>(device, tex_size, tex_size, &pixels[0], 1);
}

uint32_t Font::GetHeight() const
{
    return m_Height;
}

int Font::GetAscent() const
{
    return m_Ascent;
}

int Font::GetDescent() const
{
    return m_Descent;
}

const Ref<Texture2D>& Font::GetAtlas() const
{
    return m_Atlas;
}

const CharInfo& Font::GetCharInfo(char c) const
{
    return m_CharsInfo[c - FirstChar];
}
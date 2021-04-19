#pragma once

class Color
{
public:
    static Color FromHex(uint32_t hex);
    static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    static const Color Clear;
    static const Color White;
    static const Color Yellow;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Magenta;
    static const Color Gray;
    static const Color Grey;
    static const Color Black;

    Color();
    Color(float r, float g, float b, float a = 1.0f);

    float R, G, B, A;
};
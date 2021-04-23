#include "stdafx.h"
#include "Color.h"

Color Color::FromHex(uint32_t hex, uint8_t a)
{
    const uint8_t r = (hex >> 16) & 0xff;
    const uint8_t g = (hex >>  8) & 0xff;
    const uint8_t b =  hex        & 0xff;

    return FromRGBA(r, g, b, a);
}

Color Color::FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
}

const Color Color::Clear   = Color(0.00f, 0.00f, 0.00f, 0.00f);
const Color Color::White   = Color(1.00f, 1.00f, 1.00f, 1.00f);
const Color Color::Yellow  = Color(1.00f, 0.92f, 0.02f, 1.00f);
const Color Color::Red     = Color(1.00f, 0.00f, 0.00f, 1.00f);
const Color Color::Green   = Color(0.00f, 1.00f, 0.00f, 1.00f);
const Color Color::Blue    = Color(0.00f, 0.00f, 1.00f, 1.00f);
const Color Color::Magenta = Color(1.00f, 0.00f, 1.00f, 1.00f);
const Color Color::Gray    = Color(0.50f, 0.50f, 0.50f, 1.00f);
const Color Color::Grey    = Color(0.50f, 0.50f, 0.50f, 1.00f);
const Color Color::Black   = Color(0.00f, 0.00f, 0.00f, 1.00f);

Color::Color()
    :
    R(0.0f),
    G(0.0f),
    B(0.0f),
    A(0.0f)
{}

Color::Color(float r, float g, float b, float a)
    :
    R(r),
    G(g),
    B(b),
    A(a)
{}
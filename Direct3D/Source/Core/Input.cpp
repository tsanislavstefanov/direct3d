#include "stdafx.h"
#include "Input.h"

Vec2 Input::s_MousePosition = Vec2(0.0f);
std::bitset<8> Input::s_MouseButtonStates = 0;
std::bitset<256> Input::s_KeyStates = 0;
Vec4 Input::s_AxisRaw = Vec4(0.0f);

const Vec2& Input::GetMousePosition()
{
    return s_MousePosition;
}

bool Input::GetMouseButtonDown(size_t code)
{
    return s_MouseButtonStates[code];
}

bool Input::GetMouseButtonUp(size_t code)
{
    return !GetMouseButtonDown(code);
}

Vec2 Input::GetAxisRaw()
{
    const float x = s_AxisRaw.X + s_AxisRaw.Y;
    const float y = s_AxisRaw.Z + s_AxisRaw.W;
    return { x, y };
}

bool Input::GetKeyDown(size_t code)
{
    return s_KeyStates[code];
}

bool Input::GetKeyUp(size_t code)
{
    return !GetKeyDown(code);
}
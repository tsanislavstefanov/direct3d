#include "stdafx.h"
#include "Input.h"

////////////////////////////////
// INPUT ///////////////////////
////////////////////////////////

Vec2 Input::s_MousePosition = Vec2::Zero;
std::bitset<8> Input::s_MouseButtonStates = 0;
std::bitset<256> Input::s_KeyStates = 0;
Vec4 Input::s_AxisRaw = Vec4::Zero;

const Vec2& Input::GetMousePosition()
{
    return s_MousePosition;
}

bool Input::GetMouseButtonDown(size_t button_code)
{
    return s_MouseButtonStates[button_code];
}

bool Input::GetMouseButtonUp(size_t button_code)
{
    return !GetMouseButtonDown(button_code);
}

Vec2 Input::GetAxisRaw()
{
    return { s_AxisRaw.X + s_AxisRaw.Y, s_AxisRaw.Z + s_AxisRaw.W };
}

bool Input::GetKeyDown(size_t key_code)
{
    return s_KeyStates[key_code];
}

bool Input::GetKeyUp(size_t key_code)
{
    return !GetKeyDown(key_code);
}
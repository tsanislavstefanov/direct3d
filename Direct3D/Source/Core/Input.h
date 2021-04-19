#pragma once

#include "Math/Vector.h"

////////////////////////////////
// MOUSE BUTTON CODE(S) ////////
////////////////////////////////

namespace MouseButtonCode
{
    enum : size_t
    {
        UNKNOWN = 0x00,
        LEFT = 0x01,
        MIDDLE = 0x04,
        RIGHT = 0x02
    };
}

////////////////////////////////
// KEY CODE(S) /////////////////
////////////////////////////////

namespace KeyCode
{
    enum : size_t
    {
        UNKNOWN = 0x00,
        LEFT_ARROW = 0x25,
        UP_ARROW = 0x26,
        RIGHT_ARROW = 0x27,
        DOWN_ARROW = 0x28,
        ALPHA0 = 0x30,
        ALPHA1 = 0x31,
        ALPHA2 = 0x32,
        ALPHA3 = 0x33,
        ALPHA4 = 0x34,
        ALPHA5 = 0x35,
        ALPHA6 = 0x36,
        ALPHA7 = 0x37,
        ALPHA8 = 0x38,
        ALPHA9 = 0x39,
        A = 0x41,
    };
}

////////////////////////////////
// INPUT ///////////////////////
////////////////////////////////

class Input
{
public:
    Input() = delete;
    Input(const Input&) = delete;

    Input& operator=(const Input&) = delete;

    static const Vec2& GetMousePosition();
    static bool GetMouseButtonDown(size_t code);
    static bool GetMouseButtonUp(size_t code);
    static Vec2 GetAxisRaw();
    static bool GetKeyDown(size_t code);
    static bool GetKeyUp(size_t code);
private:
    static Vec2 s_MousePosition;
    static std::bitset<8> s_MouseButtonStates;
    static std::bitset<256> s_KeyStates;
    static Vec4 s_AxisRaw;

    friend class Application;
};
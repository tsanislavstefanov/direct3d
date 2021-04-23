#pragma once

#include "Math/Vector.h"

////////////////////////////////
// MOUSE BUTTON CODE(S) ////////
////////////////////////////////

namespace MouseButtonCode
{
    enum : size_t
    {
        Unknown = 0x00,
        Left = 0x01,
        Middle = 0x04,
        Right = 0x02
    };
}

////////////////////////////////
// KEY CODE(S) /////////////////
////////////////////////////////

namespace KeyCode
{
    enum : size_t
    {
        Unknown = 0x00,
        LeftArrow = 0x25,
        UpArrow = 0x26,
        RightArrow = 0x27,
        DownArrow = 0x28,
        Alpha0 = 0x30,
        Alpha1 = 0x31,
        Alpha2 = 0x32,
        Alpha3 = 0x33,
        Alpha4 = 0x34,
        Alpha5 = 0x35,
        Alpha6 = 0x36,
        Alpha7 = 0x37,
        Alpha8 = 0x38,
        Alpha9 = 0x39,
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
    static bool GetMouseButtonDown(size_t button_code);
    static bool GetMouseButtonUp(size_t button_code);
    static Vec2 GetAxisRaw();
    static bool GetKeyDown(size_t key_code);
    static bool GetKeyUp(size_t key_code);
private:
    static Vec2 s_MousePosition;
    static std::bitset<8> s_MouseButtonStates;
    static std::bitset<256> s_KeyStates;
    static Vec4 s_AxisRaw;

    friend class Application;
};
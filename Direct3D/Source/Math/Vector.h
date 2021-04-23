#pragma once

////////////////////////////////
// VECTOR 2 ////////////////////
////////////////////////////////

struct Vec2
{
    static const Vec2 Zero;

    Vec2();
    Vec2(float s);
    Vec2(float x, float y);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;

    bool operator==(const Vec2& v) const;
    bool operator!=(const Vec2& v) const;

    Vec2& operator+=(const Vec2& v);
    Vec2& operator-=(const Vec2& v);
    Vec2& operator*=(float s);
    Vec2& operator/=(float s);

    Vec2 operator+() const;
    Vec2 operator-() const;
    Vec2 operator+(const Vec2& v) const;
    Vec2 operator-(const Vec2& v) const;
    Vec2 operator*(float s) const;
    Vec2 operator/(float s) const;

    float X, Y;
};

////////////////////////////////
// VECTOR 3 ////////////////////
////////////////////////////////

struct Vec3
{
    Vec3();
    Vec3(float s);
    Vec3(float x, float y, float z);
    Vec3(const Vec2& v, float z);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;

    bool operator==(const Vec3& v);
    bool operator!=(const Vec3& v);

    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    Vec3& operator*=(float s);
    Vec3& operator/=(float s);

    Vec3 operator+() const;
    Vec3 operator-() const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float s) const;
    Vec3 operator/(float s) const;

    float X, Y, Z;
};

////////////////////////////////
// VECTOR 4 ////////////////////
////////////////////////////////

struct Vec4
{
    static const Vec4 Zero;

    Vec4();
    Vec4(float s);
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& v, float w);

    float& operator[](size_t index);
    const float& operator[](size_t index) const;

    bool operator==(const Vec4& v);
    bool operator!=(const Vec4& v);

    Vec4& operator+=(const Vec4& v);
    Vec4& operator-=(const Vec4& v);
    Vec4& operator*=(float s);
    Vec4& operator/=(float s);

    Vec4 operator+(const Vec4& v) const;
    Vec4 operator-(const Vec4& v) const;
    Vec4 operator*(float s) const;
    Vec4 operator/(float s) const;

    float X, Y, Z, W;
};
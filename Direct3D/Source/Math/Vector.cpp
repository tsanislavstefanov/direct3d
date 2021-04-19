#include "stdafx.h"
#include "Vector.h"

////////////////////////////////
// VECTOR 2 ////////////////////
////////////////////////////////

const Vec2 Vec2::Zero = Vec2(0.0f);

Vec2::Vec2()
    :
    X(0.0f),
    Y(0.0f)
{}

Vec2::Vec2(float s)
    :
    X(s),
    Y(s)
{}

Vec2::Vec2(float x, float y)
    :
    X(x),
    Y(y)
{}

float& Vec2::operator[](size_t index)
{
    assert(index < 2);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
    }
}

const float& Vec2::operator[](size_t index) const
{
    assert(index < 2);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
    }
}

bool Vec2::operator==(const Vec2& v) const
{
    return X == v.X && Y == v.Y;
}

bool Vec2::operator!=(const Vec2& v) const
{
    return !((*this) == v);
}

Vec2& Vec2::operator+=(const Vec2& v)
{
    X += v.X;
    Y += v.Y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& v)
{
    X -= v.X;
    Y -= v.Y;
    return *this;
}

Vec2& Vec2::operator*=(float s)
{
    X *= s;
    Y *= s;
    return *this;
}

Vec2& Vec2::operator/=(float s)
{
    X /= s;
    Y /= s;
    return *this;
}

Vec2 Vec2::operator+() const
{
    const float x = +X;
    const float y = +Y;
    return { x, y };
}

Vec2 Vec2::operator-() const
{
    const float x = -X;
    const float y = -Y;
    return { x, y };
}

Vec2 Vec2::operator+(const Vec2& v) const
{
    const float x = X + v.X;
    const float y = Y + v.Y;
    return { x, y };
}

Vec2 Vec2::operator-(const Vec2& v) const
{
    const float x = X - v.X;
    const float y = Y - v.Y;
    return { x, y };
}

Vec2 Vec2::operator*(float s) const
{
    const float x = X * s;
    const float y = Y * s;
    return { x, y };
}

Vec2 Vec2::operator/(float s) const
{
    const float x = X / s;
    const float y = Y / s;
    return { x, y };
}

////////////////////////////////
// VECTOR 3 ////////////////////
////////////////////////////////

Vec3::Vec3()
    :
    X(0.0f),
    Y(0.0f),
    Z(0.0f)
{}

Vec3::Vec3(float s)
    :
    X(s),
    Y(s),
    Z(s)
{}

Vec3::Vec3(float x, float y, float z)
    :
    X(x),
    Y(y),
    Z(z)
{}

Vec3::Vec3(const Vec2 & v, float z)
    :
    X(v.X),
    Y(v.Y),
    Z(z)
{}

float& Vec3::operator[](size_t index)
{
    assert(index < 3);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
        case 2: return Z;
    }
}

const float& Vec3::operator[](size_t index) const
{
    assert(index < 3);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
        case 2: return Z;
    }
}

bool Vec3::operator==(const Vec3& v)
{
    return X == v.X && Y == v.Y && Z == v.Z;
}

bool Vec3::operator!=(const Vec3& v)
{
    return !((*this) == v);
}

Vec3& Vec3::operator+=(const Vec3& v)
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    return *this;
}

Vec3& Vec3::operator*=(float s)
{
    X *= s;
    Y *= s;
    Z /= s;
    return *this;
}

Vec3& Vec3::operator/=(float s)
{
    X /= s;
    Y /= s;
    Z /= s;
    return *this;
}

Vec3 Vec3::operator+() const
{
    const float x = +X;
    const float y = +Y;
    const float z = +Z;
    return { x, y, z };
}

Vec3 Vec3::operator-() const
{
    const float x = -X;
    const float y = -Y;
    const float z = -Z;
    return { x, y, z };
}

Vec3 Vec3::operator+(const Vec3& v) const
{
    const float x = X + v.X;
    const float y = Y + v.Y;
    const float z = Z + v.Z;
    return { x, y, z };
}

Vec3 Vec3::operator-(const Vec3& v) const
{
    const float x = X - v.X;
    const float y = Y - v.Y;
    const float z = Z - v.Z;
    return { x, y, z };
}

Vec3 Vec3::operator*(float s) const
{
    const float x = X * s;
    const float y = Y * s;
    const float z = Z * s;
    return { x, y, z };
}

Vec3 Vec3::operator/(float s) const
{
    const float x = X / s;
    const float y = Y / s;
    const float z = Z / s;
    return { x, y, z };
}

////////////////////////////////
// VECTOR 4 ////////////////////
////////////////////////////////

const Vec4 Vec4::Zero = Vec4(0.0f);

Vec4::Vec4()
    :
    Y(0.0f),
    X(0.0f),
    Z(0.0f),
    W(0.0f)
{}

Vec4::Vec4(float s)
    :
    X(s),
    Y(s),
    Z(s),
    W(s)
{}

Vec4::Vec4(float x, float y, float z, float w)
    :
    X(x),
    Y(y),
    Z(z),
    W(w)
{}

Vec4::Vec4(const Vec3 & v, float w)
    :
    X(v.X),
    Y(v.Y),
    Z(v.Z),
    W(w)
{}

float& Vec4::operator[](size_t index)
{
    assert(index < 4);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
        case 2: return Z;
        case 3: return W;
    }
}

const float& Vec4::operator[](size_t index) const
{
    assert(index < 4);
    switch (index)
    {
        case 0: return X;
        case 1: return Y;
        case 2: return Z;
        case 3: return W;
    }
}

bool Vec4::operator==(const Vec4& v)
{
    return X == v.X && Y == v.Y && Z == v.Z && W == v.W;
}

bool Vec4::operator!=(const Vec4& v)
{
    return !((*this) == v);
}

Vec4& Vec4::operator+=(const Vec4& v)
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    W += v.W;
    return *this;
}

Vec4& Vec4::operator-=(const Vec4& v)
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    W -= v.W;
    return *this;
}

Vec4& Vec4::operator*=(float s)
{
    X *= s;
    Y *= s;
    Z *= s;
    W *= s;
    return *this;
}

Vec4& Vec4::operator/=(float s)
{
    X /= s;
    Y /= s;
    Z /= s;
    W /= s;
    return *this;
}

Vec4 Vec4::operator+(const Vec4& v) const
{
    const float x = X + v.X;
    const float y = Y + v.Y;
    const float z = Z + v.Z;
    const float w = W + v.W;
    return { x, y, z, w };
}

Vec4 Vec4::operator-(const Vec4& v) const
{
    const float x = X - v.X;
    const float y = Y - v.Y;
    const float z = Z - v.Z;
    const float w = W - v.W;
    return { x, y, z, w };
}

Vec4 Vec4::operator*(float s) const
{
    const float x = X * s;
    const float y = Y * s;
    const float z = Z * s;
    const float w = W * s;
    return { x, y, z, w };
}

Vec4 Vec4::operator/(float s) const
{
    const float x = X / s;
    const float y = Y / s;
    const float z = Z / s;
    const float w = W / s;
    return { x, y, z, w };
}
#pragma once

#include "Vector.h"

#ifdef far
    #undef far
#endif
#ifdef near
    #undef near
#endif

struct Mat4
{
    static Mat4 Ortho(float left, float right, float bottom, float top, float far, float near);
    static Mat4 Translate(const Mat4& m, const Vec3& v);
    static Mat4 Scale(const Mat4& m, const Vec3& v);
    static Mat4 Transpose(const Mat4& m);

    Mat4();
    Mat4(float s);

    Vec4& operator[](size_t index);
    const Vec4& operator[](size_t index) const;

    Vec4 Columns[4];
};

static Vec4 operator*(const Mat4& lhs, const Vec4& rhs)
{
    Vec4 r = { 0.0f };

    r += lhs[0] * rhs[0];
    r += lhs[1] * rhs[1];
    r += lhs[2] * rhs[2];
    r += lhs[3] * rhs[3];

    return r;
}

static Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
    Mat4 r = { 0.0f };

    r[0] = lhs[0] * rhs[0][0] + lhs[1] * rhs[0][1] + lhs[2] * rhs[0][2] + lhs[3] * rhs[0][3];
    r[1] = lhs[0] * rhs[1][0] + lhs[1] * rhs[1][1] + lhs[2] * rhs[1][2] + lhs[3] * rhs[1][3];
    r[2] = lhs[0] * rhs[2][0] + lhs[1] * rhs[2][1] + lhs[2] * rhs[2][2] + lhs[3] * rhs[2][3];
    r[3] = lhs[0] * rhs[3][0] + lhs[1] * rhs[3][1] + lhs[2] * rhs[3][2] + lhs[3] * rhs[3][3];

    return r;
}
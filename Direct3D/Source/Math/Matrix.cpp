#include "stdafx.h"
#include "Matrix.h"

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float far, float near)
{
    Mat4 r(1.0f);

    const float a = right - left;
    const float b = top   - bottom;
    const float c = far   - near;

    r[0][0] =  2.0f / a;
    r[1][1] =  2.0f / b;
    r[2][2] = -2.0f / c;

    r[3][0] = -(right + left)   / a;
    r[3][1] = -(top   + bottom) / b;
    r[3][2] = -(far   + near)   / c;

    return r;
}

Mat4 Mat4::Translate(const Mat4& m, const Vec3& v)
{
    Mat4 r(m);

    r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

    return r;
}

Mat4 Mat4::Scale(const Mat4& m, const Vec3& v)
{
    Mat4 r(m);

    r[0] = m[0] * v[0];
    r[1] = m[1] * v[1];
    r[2] = m[2] * v[2];

    return r;
}

Mat4 Mat4::Transpose(const Mat4& m)
{
    Mat4 r(1.0f);

    r[0][0] = m[0][0];
    r[0][1] = m[1][0];
    r[0][2] = m[2][0];
    r[0][3] = m[3][0];

    r[1][0] = m[0][1];
    r[1][1] = m[1][1];
    r[1][2] = m[2][1];
    r[1][3] = m[3][1];

    r[2][0] = m[0][2];
    r[2][1] = m[1][2];
    r[2][2] = m[2][2];
    r[2][3] = m[3][2];

    r[3][0] = m[0][3];
    r[3][1] = m[1][3];
    r[3][2] = m[2][3];
    r[3][3] = m[3][3];

    return r;
}

Mat4::Mat4()
{
    memset(&Columns, 0, sizeof(Vec4) * 4);
}

Mat4::Mat4(float s)
{
    for (size_t i = 0; i < 4; i++)
        Columns[i][i] = s;
}

Vec4& Mat4::operator[](size_t index)
{
    assert(index < 4);
    return Columns[index];
}

const Vec4& Mat4::operator[](size_t index) const
{
    assert(index < 4);
    return Columns[index];
}
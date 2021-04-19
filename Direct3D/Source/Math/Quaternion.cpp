#include "stdafx.h"
#include "Quaternion.h"

// Quat::Quat()
//     :
//     X(0.0f),
//     Y(0.0f),
//     Z(0.0f),
//     W(1.0f)
// {}
// 
// Quat::Quat(float x, float y, float z, float w)
//     :
//     X(x),
//     Y(y),
//     Z(z),
//     W(w)
// {}
// 
// Quat::Quat(const Vec3& xyz, float angle)
// {
//     W = cos(angle * 0.5f);
//     const float n = sin(angle * 0.5f);
//     X = xyz.X * n;
//     Y = xyz.Y * n;
//     Z = xyz.Z * n;
// }
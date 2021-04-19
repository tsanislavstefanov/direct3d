#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"

////////////////////////////////
// CAMERA //////////////////////
////////////////////////////////

class Camera
{
public:
    virtual ~Camera() = default;
    virtual const Mat4& GetViewProj() const = 0;
};

////////////////////////////////
// ORTHO. CAMERA ///////////////
////////////////////////////////

class OrthoCamera : public Camera
{
public:
    OrthoCamera(float left, float right, float bottom, float top);

    const Mat4& GetViewProj() const override;

    void SetProj(float left, float right, float bottom, float top);
    void SetPosition(const Vec3& position);
private:
    void UpdateViewProj();

    Mat4 m_View = Mat4(1.0f);
    Mat4 m_Proj, m_ViewProj;
    Vec3 m_Position = Vec3(0.0f);
};
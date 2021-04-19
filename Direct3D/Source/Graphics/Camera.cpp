#include "stdafx.h"
#include "Camera.h"

////////////////////////////////
// ORTHO. CAMERA ///////////////
////////////////////////////////

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
    :
    m_Proj(Mat4::Ortho(left, right, bottom, top, -1.0f, 1.0f)),
    m_ViewProj(m_Proj * m_View)
{}

const Mat4& OrthoCamera::GetViewProj() const
{
    return m_ViewProj;
}

void OrthoCamera::SetProj(float left, float right, float bottom, float top)
{
    m_Proj = Mat4::Ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProj = m_Proj * m_View;
}

void OrthoCamera::SetPosition(const Vec3& position)
{
    m_Position = position;
    UpdateViewProj();
}

void OrthoCamera::UpdateViewProj()
{
    m_View = Mat4::Translate({ 1.0f }, -m_Position);
    m_ViewProj = m_Proj * m_View;
}
#include "stdafx.h"
#include "Blender.h"

Blender::Blender(ID3D11Device* device, bool enable)
{
    D3D11_BLEND_DESC bd;
    memset(&bd, 0, sizeof(D3D11_BLEND_DESC));
    D3D11_RENDER_TARGET_BLEND_DESC& rtbd = bd.RenderTarget[0];
    if (enable)
    {
        rtbd.BlendEnable = TRUE;
        rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        rtbd.BlendOp = D3D11_BLEND_OP_ADD;
        rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;
        rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    else
    {
        rtbd.BlendEnable = FALSE;
        rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    device->CreateBlendState(&bd, &m_BlendState);
}

void Blender::Bind(ID3D11DeviceContext* device_context)
{
    device_context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xffffffff);
}
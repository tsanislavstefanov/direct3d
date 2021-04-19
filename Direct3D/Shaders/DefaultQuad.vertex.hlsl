struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEX_COORD;
    float TexIndex : TEX_INDEX;
};

cbuffer ViewProjBuffer
{
    matrix ViewProj;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEX_COORD;
    float TexIndex : TEX_INDEX;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Position = mul(input.Position, ViewProj);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    output.TexIndex = input.TexIndex;
    return output;
}
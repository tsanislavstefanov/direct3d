struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
    float2 TexCoord : TEX_COORD;
    float TexIndex : TEX_INDEX;
};

Texture2D g_Tex2Ds[4];
SamplerState g_SamplerState;

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixel = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    const int index = int(input.TexIndex);
    switch (index)
    {
        case 0:
            pixel = g_Tex2Ds[0].Sample(g_SamplerState, input.TexCoord);
            break;
        case 1:
            pixel.a = g_Tex2Ds[1].Sample(g_SamplerState, input.TexCoord).r;
            break;
        case 2:
            pixel = g_Tex2Ds[2].Sample(g_SamplerState, input.TexCoord);
            break;
        case 3:
            pixel = g_Tex2Ds[3].Sample(g_SamplerState, input.TexCoord);
            break;
    }
    
    return pixel * input.Color;
}
#include "../ShaderStructs.hlsli"

cbuffer EmissiveBuffer : register(b8)
{
    float myIntensity;
    float3 padding;
}

PixelOutput main(FullscreenVertexToPixel input)
{
    float4 albedoData = myAlbedoTexture.Sample(myDefaultSampler, input.myUV);
    if (albedoData.a == 0)
    {
        discard;
    }

    float3 albedo = albedoData.rgb;
    float4 material = myMaterialTexture.Sample(myDefaultSampler, input.myUV);
    float emissiveMask = material.b;

    PixelOutput output;
    output.myColor.rgb = albedo * emissiveMask * myIntensity;
    output.myColor.a = 1.0f;
    return output;
}
#include "FullscreenStructs.hlsli"
#include "../PBRFunctions.hlsli"

/*
    // Textures
    myFullscreenTexture1 Albedo
    myFullscreenTexture2 Material
    myFullscreenTexture3 Normal

    // Passes
    1. Albedo
    2. Roughness
    3. Metalness
    4. Ao
    5. Emissive
*/

cbuffer PassBuffer : register(b8)
{
    int myPass = 0;

    int3 padding;
};

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    if (myPass == 1)
    {
        output.myColor.rgb = LinearToGamma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 2)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).g;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 3)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).r;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 4)
    {
        float val = myFullscreenTexture3.Sample(myDefaultSampler, input.myUV).b;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 5)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).b;
        output.myColor.rgb = (val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    output.myColor = 0;
    return output;
}
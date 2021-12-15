#include "FullscreenStructs.hlsli"
#include "../PBRFunctions.hlsli"

/*
    // Textures
    myFullscreenTexture1 Albedo
    myFullscreenTexture2 Material
    myFullscreenTexture3 Normal
    myFullscreenTexture4 AO

    // Passes
    1. Albedo
    2. Normal
    3. Roughness
    4. Metalness
    5. Ao
    6. Emissive
    7. NormalTexture

    // Den heliga bilden på Tga Standard.
    https://cdn.discordapp.com/attachments/789538022134644807/912792323233030164/unknown.png
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
        output.myColor.rgb = myFullscreenTexture3.Sample(myDefaultSampler, input.myUV).rgb;
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 3)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).g;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 4)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).r;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 5)
    {
        float val = myFullscreenTexture4.Sample(myDefaultSampler, input.myUV).r;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 6)
    {
        float val = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).b;
        output.myColor.rgb = float3(val, val, val);
        output.myColor.a = 1.0f;
        return output;
    }

    if (myPass == 7)
    {
        output.myColor.rgb = myFullscreenTexture5.Sample(myDefaultSampler, input.myUV).rgb;
        output.myColor.a = 1.0f;
        return output;
    }

    output.myColor = 0;
    return output;
}
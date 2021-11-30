#include "FullscreenStructs.hlsli"

float3 Tonemap(float3 aColor)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    return ((aColor * (A * aColor + C * B) + D * E) / (aColor * (A * aColor + B) + D * F)) - E / F;
}

float3 Average(float3 aColor)
{
    return aColor / (aColor + 1.0f);
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;
    float3 hdr = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;
    float3 ldr = Tonemap(hdr);

    result.myColor.rgb = ldr;
    result.myColor.a = 1.0f;
    return result;
}
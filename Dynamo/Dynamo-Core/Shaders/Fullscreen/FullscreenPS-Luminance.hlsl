#include "FullscreenStructs.hlsli"

cbuffer LuminanceBuffer : register(b8)
{
    float myCutoff;
    float3 padding;
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;
    result.myColor = 0;

    float3 resource = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;
    float brightness = dot(resource, float3(0.2126, 0.7152, 0.0722));

    if (brightness > myCutoff)
    {
        result.myColor.rgb = resource.rgb;
    }

    result.myColor.a = 1.0f;
    return result;
}
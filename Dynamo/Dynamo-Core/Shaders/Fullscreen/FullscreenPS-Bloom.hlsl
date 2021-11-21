#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;

    float3 resource1 = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;
    float3 resource2 = myFullscreenTexture2.Sample(myDefaultSampler, input.myUV).rgb;

	{
        float luminance = dot(resource1, float3(0.2126f, 0.7152f, 0.0722f));
        float3 scaledResource = resource2 * (1.0f - luminance);
        result.myColor.rgb = resource1 + resource2;
    }

    result.myColor.a = 1.0f;

    return result;
}
#include "FullscreenStructs.hlsli"

cbuffer LuminanceBuffer : register(b8)
{
    float myCutoff;
    float3 padding;
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput result;
    float3 resource = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;

	{
        float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));

        if (luminance >= myCutoff)
        {
            result.myColor.rgb = resource;
        }
        else if (luminance >= myCutoff * .5f)
        {
            float fade = luminance / myCutoff;
            fade = pow(fade, 5);
            result.myColor.rgb = resource * fade;
        }
        else
        {
            result.myColor.rgb = 0;
        }
    }

    result.myColor.a = 1.0f;
    return result;
}
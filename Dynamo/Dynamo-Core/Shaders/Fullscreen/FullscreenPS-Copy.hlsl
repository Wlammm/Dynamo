#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    returnValue.myColor.rgb = myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb;
    returnValue.myColor.a = 1.0f;
    return returnValue;
}
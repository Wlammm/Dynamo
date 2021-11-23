#include "FullscreenStructs.hlsli"
#include "../PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    output.myColor.rgb = LinearToGamma(myFullscreenTexture1.Sample(myDefaultSampler, input.myUV).rgb);
    output.myColor.a = 1.0f;
    return output;
}
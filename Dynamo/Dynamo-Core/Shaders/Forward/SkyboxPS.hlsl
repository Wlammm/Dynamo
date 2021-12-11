#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"

PixelOutput main(VertexOutput input)
{
    PixelOutput output;

    float3 dir = input.myWorldPosition - myFrameBuffer.myCameraPosition;
    output.myColor.rgb = myCubeMap.Sample(myDefaultSampler, dir);
    output.myColor.a = 1.0f;

    return output;
}
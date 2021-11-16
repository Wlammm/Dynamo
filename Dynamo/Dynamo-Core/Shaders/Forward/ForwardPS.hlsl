#include "../ShaderStructs.hlsli"

PixelOutput main(VertexOutput input)
{
    float4 albedo = myAlbedoTexture.Sample(myDefaultSampler, input.myUV0);
    
    PixelOutput output;
    output.myColor = albedo;
    output.myColor.a = 1;

    return output;
}
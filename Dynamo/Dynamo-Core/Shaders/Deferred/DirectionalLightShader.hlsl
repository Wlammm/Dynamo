#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"

PixelOutput main(FullscreenVertexToPixel input)
{
    float4 albedoData = myAlbedoTexture.Sample(myDefaultSampler, input.myUV);
    if (albedoData.a == 0)
    {
        discard;
    }
    
    float3 albedo = albedoData.rgb;
    float4 worldPosition = myPositionTexture.Sample(myDefaultSampler, input.myUV);
    float3 normal = myNormalTexture.Sample(myDefaultSampler, input.myUV).xyz;
    float3 vertexNormal = myVertexNormalTexture.Sample(myDefaultSampler, input.myUV).xyz;
    float4 material = myMaterialTexture.Sample(myDefaultSampler, input.myUV);
    
    float metalness = material.r;
    float roughness = material.g;
    float emissiveMask = material.b;
    
    float ao = myAmbientOcclusionTexture.Sample(myDefaultSampler, input.myUV).r;
    float depth = myDepthTexture.Sample(myDefaultSampler, input.myUV).r;
    
    float3 toEye = normalize(myFrameBuffer.myCameraPosition.xyz - worldPosition.xyz);
    
    float3 specColor = lerp((float3) 0.04f, albedo, metalness);
    float3 diffColor = lerp((float3) 0.00f, albedo, 1 - metalness);
    
    float3 directionalLight = EvaluateDirectionalLight(diffColor, specColor, normal, roughness, myDirectionalLightBuffer.myColor.rgb, myDirectionalLightBuffer.myToLight.xyz, toEye.xyz);
    
    float3 emissive = albedo * emissiveMask;
    float3 radiance = directionalLight * myDirectionalLightBuffer.myIntensity + emissive;
    
    PixelOutput output;
    output.myColor.rgb = LinearToGamma(radiance);
    output.myColor.a = 1.0f;
    return output;
}

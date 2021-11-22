#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"

cbuffer SpotLightBuffer : register(b7)
{
    struct SpotLightBuffer
    {
        float4 myPosition;
        float4 myDirection;
        float4 myColor;
        float myIntensity;
        float myRange;
        float myInnerAngle;
        float myOuterAngle;
    } mySpotLightBuffer;
}

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

    float ao = myAmbientOcclusionTexture.Sample(myDefaultSampler, input.myUV).r;
    float depth = myDepthTexture.Sample(myDefaultSampler, input.myUV).r;

    float3 toEye = normalize(myFrameBuffer.myCameraPosition.xyz - worldPosition.xyz);

    float3 specColor = lerp((float3) 0.04, albedo, metalness);
    float3 diffColor = lerp((float3) 0.00, albedo, 1 - metalness);

    float3 spotLights = EvaluateSpotLight(diffColor, specColor, normal, roughness, mySpotLightBuffer.myColor.rgb, mySpotLightBuffer.myIntensity, mySpotLightBuffer.myRange, 
        mySpotLightBuffer.myPosition.xyz, mySpotLightBuffer.myDirection.xyz, mySpotLightBuffer.myOuterAngle, mySpotLightBuffer.myInnerAngle, toEye.xyz, worldPosition.xyz);
    
    PixelOutput output;
    output.myColor.rgb = spotLights;
    output.myColor.a = 1.0f;

    return output;
}
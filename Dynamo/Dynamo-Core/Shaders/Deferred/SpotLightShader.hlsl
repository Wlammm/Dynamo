#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"
#include "../SharedDefines.hpp"

cbuffer SpotLightBuffer : register(b7)
{
    struct SpotLightBuffer
    {
        struct
        {
            float4 myPosition;
            float4 myDirection;
            float4 myColor;
            float myIntensity;
            float myRange;
            float myInnerAngle;
            float myOuterAngle;
        } myLights[DEFERRED_SPOT_LIGHT_COUNT];

        uint myNumSpotLights;
        uint3 padding;
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

    float3 spotLight = 0;

    for (uint i = 0; i < mySpotLightBuffer.myNumSpotLights; ++i)
    {
        spotLight += EvaluateSpotLight(diffColor, specColor, normal, roughness, mySpotLightBuffer.myLights[i].myColor.rgb, mySpotLightBuffer.myLights[i].myIntensity,
            mySpotLightBuffer.myLights[i].myRange, mySpotLightBuffer.myLights[i].myPosition.xyz, mySpotLightBuffer.myLights[i].myDirection.xyz,
            mySpotLightBuffer.myLights[i].myOuterAngle, mySpotLightBuffer.myLights[i].myInnerAngle, toEye.xyz, worldPosition.xyz);
    }
    
    PixelOutput output;
    output.myColor.rgb = spotLight;
    output.myColor.a = 1.0f;

    return output;
}
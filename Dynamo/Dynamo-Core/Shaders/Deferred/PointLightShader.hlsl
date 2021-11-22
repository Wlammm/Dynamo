#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"

cbuffer PointLightBuffer : register(b6)
{
    struct PointLightBuffer
    {
        struct
        {
            float4 myPosition;
            float4 myColor;
            float myIntensity;
            float myRange;
            float2 padding;
        } lights[32];
        
        uint myNumLights;
        uint3 padding;
    } myPointLightBuffer;
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

    float3 pointLight = 0;
    
    for (int i = 0; i < myPointLightBuffer.myNumLights; ++i)
    {
        pointLight = EvaluatePointLight(diffColor, specColor, normal, roughness, myPointLightBuffer.lights[i].myColor.rgb, myPointLightBuffer.lights[i].myIntensity, myPointLightBuffer.lights[i].myRange,
            myPointLightBuffer.lights[i].myPosition.xyz, toEye.xyz, worldPosition.xyz);
    }

    PixelOutput output;
    output.myColor.rgb = pointLight;
    output.myColor.a = 1.0f;

    return output;
}
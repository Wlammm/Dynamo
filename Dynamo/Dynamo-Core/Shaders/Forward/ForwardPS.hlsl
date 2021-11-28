#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"
#include "../SharedDefines.hpp"

float3 GetAlbedo(VertexOutput input)
{
    float3 albedo = myAlbedoTexture.Sample(myDefaultSampler, input.myUV0).rgb;
    return GammaToLinear(albedo);
}

float3 GetNormal(VertexOutput input)
{
    float3 normal = float3(myNormalTexture.Sample(myDefaultSampler, input.myUV0).wy, 0);
    normal = 2.0f * normal - 1.0f;
    normal.z = sqrt(1.0f - saturate(normal.x * normal.x + normal.y * normal.y));

    float3x3 TBN = float3x3(
        normalize(input.myTangent.xyz),
        normalize(input.myBinormal.xyz),
        normalize(input.myNormal.xyz));
    
    normal = mul(normal.xyz, TBN);
    normal = normalize(normal);
    
    return normal;
}

float GetAO(VertexOutput input)
{
    return myNormalTexture.Sample(myDefaultSampler, input.myUV0).b;
}

float GetMetalness(VertexOutput input)
{
    return myMaterialTexture.Sample(myDefaultSampler, input.myUV0).r;
}

float GetPRoughness(VertexOutput input)
{
    return myMaterialTexture.Sample(myDefaultSampler, input.myUV0).g;
}

float GetEmissive(VertexOutput input)
{
    return myMaterialTexture.Sample(myDefaultSampler, input.myUV0).b;
}

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
        } myLights[FORWARD_POINT_LIGHT_COUNT];
        
        uint myLightCount;
        uint3 padding;
    } myPointLightBuffer;
}

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
        } myLights[FORWARD_SPOT_LIGHT_COUNT];

        uint myNumSpotLights;
        uint3 padding;
    } mySpotLightBuffer;
}

PixelOutput main(VertexOutput input)
{
    float3 toEye = normalize(myFrameBuffer.myCameraPosition.xyz - input.myWorldPosition.xyz);
    float3 albedo = GetAlbedo(input);
    float3 normal = GetNormal(input);
    float ao = GetAO(input);
    float metalness = GetMetalness(input);
    float pRoughness = GetPRoughness(input);
    float emissiveData = GetEmissive(input);
    
    float3 specColor = lerp((float3) 0.04f, albedo, metalness);
    float3 difColor = lerp((float3) 0.0f, albedo, 1.0f - metalness);
    
    float3 ambience = EvaluateAmbience(myCubeMap, normal, input.myNormal.xyz, toEye, pRoughness, ao, difColor, specColor) * myAmbientLightBuffer.myIntensity;
    float3 dirLight = EvaluateDirectionalLight(difColor, specColor, normal, pRoughness, myDirectionalLightBuffer.myColor.rgb, myDirectionalLightBuffer.myToLight.xyz, toEye.xyz) * myDirectionalLightBuffer.myIntensity;

    float3 pointLights = 0;
    for (uint i = 0; i < myPointLightBuffer.myLightCount; ++i)
    {
        pointLights += EvaluatePointLight(difColor, specColor, normal, pRoughness, myPointLightBuffer.myLights[i].myColor.rgb, myPointLightBuffer.myLights[i].myIntensity,
            myPointLightBuffer.myLights[i].myRange, myPointLightBuffer.myLights[i].myPosition.xyz, toEye, input.myWorldPosition.xyz);
    }
    
    float3 spotLights = 0;
    for (uint j = 0; j < mySpotLightBuffer.myNumSpotLights; ++j)
    {
        spotLights += EvaluateSpotLight(difColor, specColor, normal, pRoughness, mySpotLightBuffer.myLights[j].myColor.rgb, mySpotLightBuffer.myLights[j].myIntensity, mySpotLightBuffer.myLights[j].myRange,
            mySpotLightBuffer.myLights[j].myPosition.xyz, mySpotLightBuffer.myLights[j].myDirection.xyz, mySpotLightBuffer.myLights[j].myOuterAngle, mySpotLightBuffer.myLights[j].myInnerAngle, 
            toEye.xyz, input.myWorldPosition.xyz);
    }
    
    float3 emissive = albedo * emissiveData;
    float3 radiance = ambience + dirLight + pointLights + spotLights + emissive;
    
    PixelOutput output;
    output.myColor.rgb = LinearToGamma(radiance);
    output.myColor.a = 1.0f;
    return output;
}
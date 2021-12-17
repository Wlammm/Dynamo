#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"
#include "../SharedDefines.hpp"

float3 vertexNormalWS(VertexOutput input)
{
    float3 scale = myMaterialBuffer.myCustomValue2;

    float3 vertexNormal = normalize(input.myNormal);
    float3 vertexNormalMod = saturate((((abs(vertexNormal) - 0.5f) * scale) + 0.5f));

    return vertexNormalMod;
}

float2 worldUV(VertexOutput input)
{
    float3 worldPos = input.myWorldPosition.xyz;
    float3 worldPosScale = worldPos / myMaterialBuffer.myCustomValue1;
    
    float3 worldNormal = normalize(input.myNormal);
    float3 worldNormalAbs = abs(worldNormal);
    
    
    float2 worldPosUV;
    
    if (worldNormalAbs.x > 0.5f)
    {
        worldPosUV = worldPosScale.zy;
    }
    else if (worldNormalAbs.y > 0.5f)
    {
        worldPosUV = worldPosScale.zx;
    }
    else
    {
        worldPosUV = worldPosScale.xy;
    }
    
    return worldPosUV;
}

float3 getAbsoluteWorldPos(VertexOutput input)
{
    float3 worldPos = input.myWorldPosition.xyz;
    float3 worldPosScale = worldPos / myMaterialBuffer.myCustomValue1;

    return worldPosScale;
}

float3 GetNormal(VertexOutput input)
{
    float3 normal = myNormalTexture.Sample(myWrapSampler, worldUV(input)).xyz;

    normal = 2.0f * normal - 1.0f;
    normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
    normal = normalize(normal);
    
    float3x3 TBN = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
    TBN = transpose(TBN);
    return normalize(mul(TBN, normal));
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

    float3 albedoXY = GammaToLinear(myAlbedoTexture.Sample(myWrapSampler, getAbsoluteWorldPos(input).xy).rgb * myObjectBuffer.myColor.rgb + myObjectBuffer.myAdditiveColor.rgb);
    float3 albedoZY = GammaToLinear(myAlbedoTexture.Sample(myWrapSampler, getAbsoluteWorldPos(input).zy).rgb * myObjectBuffer.myColor.rgb + myObjectBuffer.myAdditiveColor.rgb);
    float3 albedoZX = GammaToLinear(myAlbedoTexture.Sample(myWrapSampler, getAbsoluteWorldPos(input).zx).rgb * myObjectBuffer.myColor.rgb + myObjectBuffer.myAdditiveColor.rgb);

    float3 material = myMaterialTexture.Sample(myWrapSampler, worldUV(input)).rgb;
    float ao = myNormalTexture.Sample(myWrapSampler, worldUV(input)).b;


    float3 normal = GetNormal(input);
    material.r = lerp(myMaterialBuffer.myMetalnessConstant, material.r, myMaterialBuffer.myMetalnessInterp);
    material.g = lerp(myMaterialBuffer.myRoughnessConstant, material.g, myMaterialBuffer.myRoughnessInterp);

    float metalness = material.r;
    float pRoughness = material.g;
    float emissiveData = material.b;

    float3 albedoLerpZYXY = lerp(albedoZY, albedoXY, vertexNormalWS(input).z);
    float3 albedo = lerp(albedoLerpZYXY, albedoZX, vertexNormalWS(input).y);
    
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

    float3 emissive = albedo * emissiveData * myEmissiveBuffer.myIntensity;
    float3 radiance = ambience + dirLight + pointLights + spotLights + emissive;
    
    PixelOutput output;
    output.myColor.rgb = LinearToGamma(radiance);
    output.myColor.a = 1.0f * myObjectBuffer.myColor.a;
    return output;
}
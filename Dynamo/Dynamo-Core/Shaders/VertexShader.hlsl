#include "ShaderStructs.hlsli"

VertexOutput main(VertexInput input)
{
    VertexOutput returnValue;
    
    float4 vertexObjectPos = input.myPosition;
    float4 vertexWorldPos = mul(myObjectBuffer.myToWorld, vertexObjectPos);
    float4 vertexViewPos = mul(myFrameBuffer.myToCamera, vertexWorldPos);
    float4 vertexProjectionPos = mul(myFrameBuffer.myToProjection, vertexViewPos);
    
    float3x3 toWorldRotation = (float3x3) myObjectBuffer.myToWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.myNormal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.myTangent.xyz);
    float3 vertexWorldBinormal = mul(toWorldRotation, input.myBinormal.xyz);
    
    vertexWorldPos.w = 1.0f;
    
    returnValue.myPosition = vertexProjectionPos;
    returnValue.myWorldPosition = vertexWorldPos;
    returnValue.myNormal = float4(vertexWorldNormal, 0);
    returnValue.myTangent = float4(vertexWorldTangent, 0);
    returnValue.myBinormal = float4(vertexWorldBinormal, 0);
    return returnValue;
}
#include "ShaderStructs.hlsli"

VertexOutput main(VertexInput input)
{
    VertexOutput returnValue;

    float4 vertexObjectPos = input.myPosition.xyzw;
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
    returnValue.myDepth = 0;
    returnValue.myNormal = float4(vertexWorldNormal, 0);
    returnValue.myTangent = float4(vertexWorldTangent, 0);
    returnValue.myBinormal = float4(vertexWorldBinormal, 0);
    returnValue.myUV0 = input.myUV0;
    returnValue.myUV1 = input.myUV1;
    returnValue.myUV2 = input.myUV2;
    returnValue.myUV3 = input.myUV3;
    returnValue.myVxColor0 = input.myVxColor0;
    returnValue.myVxColor1 = input.myVxColor1;
    returnValue.myVxColor2 = input.myVxColor2;
    returnValue.myVxColor3 = input.myVxColor3;
    return returnValue;
}
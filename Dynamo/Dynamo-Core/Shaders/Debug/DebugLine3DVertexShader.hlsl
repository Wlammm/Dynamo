#include "DebugLineStructs.hlsli"

cbuffer FrameBuffer : register(b0)
{
    float4x4 myToCamera;
    float4x4 myToProjection;
}

// cbuffer ObjectBuffer : register(b1)
// {
//     float4x4 myToWorld;
// }

VertexOutput main(VertexInput aInput)
{
    VertexOutput output;

	// W must be 1 for correct matrix calculations!
    float4 vertexObjectPos = float4(aInput.myPosition.xyz, 1.f);

	// We already precalculated the vertex world pos in the vertex buffer, to prepare for batching :)
	// I mean, the line From and To are literally given in world pos already
    float4 vertexWorldPos = vertexObjectPos;
	//float4 vertexWorldPos = mul(myToWorld, vertexObjectPos);

    float4 vertexViewPos = mul(myToCamera, vertexWorldPos);
    float4 vertexProjectionPos = mul(myToProjection, vertexViewPos);

    output.myPosition = vertexProjectionPos;
    output.myColor = aInput.myColor;

    return output;
}
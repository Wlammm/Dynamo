Texture2D myAlbedoTexture : register(t1);
Texture2D myNormalTexture : register(t2);
Texture2D myMaterialTexture : register(t3);

SamplerState myDefaultSampler : register(s0);

struct VertexInput
{
    float4 myPosition : POSITION;
    float4 myVxColor0 : COLOR0;
    float4 myVxColor1 : COLOR1;
    float4 myVxColor2 : COLOR2;
    float4 myVxColor3 : COLOR3;
    float2 myUV0 : UV0;
    float2 myUV1 : UV1;
    float2 myUV2 : UV2;
    float2 myUV3 : UV3;
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBinormal : BINORMAL;
};

struct VertexOutput
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : WORLDPOSITION;
    float myDepth : DEPTH;
    float4 myColor0 : COLOR0;
    float4 myColor1 : COLOR1;
    float4 myColor2 : COLOR2;
    float4 myColor3 : COLOR3;
    float2 myUV0 : UV0;
    float2 myUV1 : UV1;
    float2 myUV2 : UV2;
    float2 myUV3 : UV3;
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBinormal : BINORMAL;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    struct FrameBuffer
    {
        float4x4 myToCamera;
        float4x4 myToProjection;
        float4 myCameraPosition;
    } myFrameBuffer;
};

cbuffer ObjectBuffer : register(b1)
{
    struct ObjectBuffer
    {
        float4x4 myToWorld;
        float4 myColor;
        float2 myUVScale;
        
        float2 padding;
    } myObjectBuffer;
}
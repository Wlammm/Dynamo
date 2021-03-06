TextureCube myCubeMap               : register(t0);

Texture2D myAlbedoTexture           : register(t1);
Texture2D myNormalTexture           : register(t2);
Texture2D myMaterialTexture         : register(t3);
Texture2D myPositionTexture         : register(t4);
Texture2D myVertexNormalTexture     : register(t5);
Texture2D myAmbientOcclusionTexture : register(t6);
Texture2D myDepthTexture            : register(t7);

SamplerState myDefaultSampler   : register(s0);
SamplerState myWrapSampler      : register(s1);
SamplerState myClampSampler     : register(s2);
SamplerState myPointSampler     : register(s3);

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
    uint4 myBoneIDs : BONEIDS;
    float4 myBoneWeights : BONEWEIGHTS;
};

struct VertexOutput
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : WORLDPOSITION;
    float myDepth : DEPTH;
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
        float4 myAdditiveColor;
        float2 myUVScale;
        int myIsAnimated;

        // These values are bound to the meshrenderer. Could differ per instance.
        float myCustomValue1;
        float myCustomValue2;
        float myCustomValue3;
        float myCustomValue4;

        float padding;
    } myObjectBuffer;
}

cbuffer BoneBuffer : register(b3)
{
    struct BoneBuffer
    {
        float4x4 myBoneTransforms[128];
    } myBoneBuffer;
}

cbuffer DirectionalLightBuffer : register(b4)
{
    struct DirectionalLightBuffer
    {
        float4 myToLight;
        float4 myColor;
        float myIntensity;

        float3 padding;
    } myDirectionalLightBuffer;
}

cbuffer AmbientLightBuffer : register(b5)
{
    struct AmbientLightBuffer
    {
        float myIntensity;
        
        float3 padding;
    } myAmbientLightBuffer;
}

cbuffer EmissiveBuffer : register(b9)
{
    struct EmissiveBuffer
    {
        float myIntensity;
        float3 padding;
    } myEmissiveBuffer;
}

cbuffer MaterialBuffer : register(b10)
{
    struct MaterialBuffer
    {
        float myRoughnessConstant;
        float myRoughnessInterp;
        float myMetalnessConstant;
        float myMetalnessInterp;
        
        // These values are bound on the material. Does not differ per instance.
        float myCustomValue1;
        float myCustomValue2;
        float myCustomValue3;
        float myCustomValue4;
    } myMaterialBuffer;
}

struct FullscreenVertexToPixel
{
    float4 myPosition : SV_POSITION;
    float2 myUV : UV;
};
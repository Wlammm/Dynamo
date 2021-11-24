#include "../ShaderStructs.hlsli"
#include "../PBRFunctions.hlsli"

struct GBufferOutput
{
    float4 myAlbedo : SV_TARGET0;
    float4 myNormal : SV_TARGET1;
    float3 myMaterial : SV_TARGET2;
    float4 myWorldPosition : SV_TARGET3;
    float4 myVertexNormal : SV_TARGET4;
    float4 myAmbientOcclusion : SV_TARGET5;
    float myDepth : SV_TARGET6;
};

GBufferOutput main(VertexOutput input)
{
    float2 scaledUV = input.myUV0 * myObjectBuffer.myUVScale;
    float3 albedo = GammaToLinear(myAlbedoTexture.Sample(myWrapSampler, scaledUV).rgb);
    
    float3 normal = myNormalTexture.Sample(myWrapSampler, scaledUV).wyz;
    float ao = normal.b;
    
    normal = 2.0f * normal - 1.0f;
    normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
    normal = normalize(normal);
    
    float3x3 TBN = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
    TBN = transpose(TBN);
    
    float3 pixelNormal = normalize(mul(TBN, normal));
    float3 material = myMaterialTexture.Sample(myWrapSampler, scaledUV).rgb;
    
    float4 outputPos = input.myWorldPosition;
    
    GBufferOutput output;
    
    output.myAlbedo = float4(albedo, 1);
    output.myNormal = float4(pixelNormal, 1);
    output.myMaterial = material;
    output.myWorldPosition = outputPos;
    output.myVertexNormal = float4(input.myNormal.xyz, 1);
    output.myAmbientOcclusion = float4(ao, ao, ao, 1.0f);
    output.myDepth = 1.0f;
    return output;
}
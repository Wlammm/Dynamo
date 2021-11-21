Texture2D myFullscreenTexture1 : register(t0);
Texture2D myFullscreenTexture2 : register(t1);
Texture2D myFullscreenTexture3 : register(t2);
Texture2D myFullscreenTexture4 : register(t3);

SamplerState myDefaultSampler : register(s0);

struct VertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct VertexToPixel
{
    float4 myPosition : SV_POSITION;
    float2 myUV : UV;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

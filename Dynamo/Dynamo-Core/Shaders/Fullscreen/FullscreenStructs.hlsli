Texture2D myFullscreenTexture1 : register(t0);
Texture2D myFullscreenTexture2 : register(t1);
Texture2D myFullscreenTexture3 : register(t2);
Texture2D myFullscreenTexture4 : register(t3);
Texture2D myFullscreenTexture5 : register(t4);
Texture2D myFullscreenTexture6 : register(t5);
Texture2D myFullscreenTexture7 : register(t6);
Texture2D myFullscreenTexture8 : register(t7);

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

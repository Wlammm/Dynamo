struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myColor : COLOR;
};

struct VertexOutput
{
	float4 myPosition : SV_POSITION;
	float4 myColor : COLOR;
};

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};
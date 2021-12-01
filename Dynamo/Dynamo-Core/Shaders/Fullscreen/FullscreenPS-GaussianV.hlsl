#include "FullscreenStructs.hlsli"
#include "../SharedDefines.hpp"

static const float2 Resolution = float2(WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y) / 4.0f;
static const float GaussianKernal5[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	float texelSize = 1.0f / Resolution.y;
	float3 blurColor = 0;

	unsigned int kernalSize = 5;
	float start = (((float)(kernalSize)-1.0f) / 2.0f) * -1.0f;
	for (unsigned int i = 0; i < kernalSize; i++)
	{
		float2 uv = input.myUV + float2(0.0f, texelSize * (start + (float)i));
		float3 resource = myFullscreenTexture1.Sample(myDefaultSampler, uv).rgb;
		blurColor += resource * GaussianKernal5[i];
	}

	result.myColor.rgb = blurColor;
	result.myColor.a = 1.0f;

	return result;
}
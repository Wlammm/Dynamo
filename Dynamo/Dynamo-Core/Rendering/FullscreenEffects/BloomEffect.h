#pragma once

#include "FullscreenEffect.h"

namespace Dynamo
{
	class BloomEffect : public FullscreenEffect
	{
	public:
		BloomEffect();

		void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget);

		void SetCutoff(const float aCutoff);
		float GetCutoff() const;

	private:
		void ClearTextures();

	private:
		struct LuminanceBuffer
		{
			float myCutoff = 0.8f;
			Vec3f padding;
		} myLuminanceBufferData;
		ID3D11Buffer* myLuminanceBuffer;

		Texture myLuminanceTexture;
		Texture myHalfSizeTexture;
		Texture myQuarterSizeTexture;
		Texture myBlurTexture1;
		Texture myBlurTexture2;
		Texture myIntermediateTexture;

		Shader* myLuminanceShader = nullptr;
		Shader* myGaussianVShader = nullptr;
		Shader* myGaussianHShader = nullptr;
		Shader* myBloomShader = nullptr;
	};
}
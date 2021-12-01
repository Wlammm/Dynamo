#pragma once

#include "FullscreenEffect.h"

namespace Dynamo
{
	class BloomEffect : public FullscreenEffect
	{
#pragma region Structs
		struct LuminanceBuffer
		{
			float myCutoff;
			Vec3f padding;
		};
#pragma endregion

	public:
		BloomEffect();

		void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget);

		void SetCutoff(const float aCutoff);
		float GetCutoff() const;

	private:
		void ClearTextures();

	private:
		float myCutoff = 1.0f;

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

		LuminanceBuffer myLuminanceBufferData;
		ID3D11Buffer* myLuminanceBuffer = nullptr;
	};
}
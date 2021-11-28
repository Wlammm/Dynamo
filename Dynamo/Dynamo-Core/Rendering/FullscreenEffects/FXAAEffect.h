#pragma once

#include "FullscreenEffect.h"

namespace Dynamo
{
	class FXAAEffect : public FullscreenEffect
	{
	public:
		FXAAEffect();
		~FXAAEffect();
		void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget) override;

	private:
		struct FXAABuffer
		{
			Vec2f myResolution;
			Vec2f padding;
		} myBufferData;

		ID3D11Buffer* myBuffer;
		Shader* myShader = nullptr;

		Texture myIntermediateTexture;
	};
}
#pragma once

#include "FullscreenEffect.h"

namespace Dynamo
{
	class HDREffect : public FullscreenEffect
	{
	public:
		HDREffect();

		void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget) override;

		void Release() override;
		void ReInit() override;

	private:
		Texture myIntermediateTexture;
		Shader* myHDRShader;
	};
}
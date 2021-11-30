#pragma once

#include "FullscreenEffect.h"

namespace Dynamo
{
	class HDREffect : public FullscreenEffect
	{
	public:
		HDREffect();

		void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget) override;

	private:
		Texture myIntermediateTexture;
		Shader* myHDRShader;
	};
}
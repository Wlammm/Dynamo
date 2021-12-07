#pragma once

namespace Dynamo
{
	class FullscreenRenderer;
	class Texture;

	class FullscreenEffect
	{
	public:
		virtual void Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget) = 0;
		virtual void Release() = 0;
		virtual void ReInit() = 0;
	};
}
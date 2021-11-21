#pragma once 

#include "Rendering/Texture.h"

namespace Dynamo
{
	class TextureFactory
	{
	public:
		static Texture CreateTexture(const Vec2ui& aSize, DXGI_FORMAT aFormat);
		static Texture CreateTexture(ID3D11Texture2D* aTexture);
		static Texture CreateDepth(const Vec2ui& aSize, DXGI_FORMAT aFormat);

	private:
	};
}
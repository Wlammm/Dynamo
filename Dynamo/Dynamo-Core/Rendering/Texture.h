#pragma once

namespace Dynamo
{
	class Texture
	{
	public:
		~Texture();
		void SetAsActiveDepth();
		void SetAsActiveTarget(Texture* aDepth = nullptr);
		void SetAsResourceOnSlot(const uint aSlot) const;


	private:
		friend class TextureFactory;

		union
		{
			ID3D11RenderTargetView* myRenderTarget;
			ID3D11DepthStencilView* myDepth;
		};

		ID3D11Texture2D* myTexture = nullptr;
		ID3D11ShaderResourceView* mySRV = nullptr;
		D3D11_VIEWPORT* myViewport = nullptr;
	};
}
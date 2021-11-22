#pragma once

namespace Dynamo
{
	class Texture
	{
	public:
		Texture() = default;
		~Texture();
		void SetAsActiveDepth();
		void SetAsActiveTarget(Texture* aDepth = nullptr);
		void SetAsResourceOnSlot(const uint aSlot) const;

		ID3D11DepthStencilView* GetDepth() const;

		void ClearTexture();
		void ClearDepth(float aClearDepthValue = 1.0f, uint aClearStencilValue = 0);

	private:
		friend class TextureFactory;

		union
		{
			ID3D11RenderTargetView* myRenderTarget = nullptr;
			ID3D11DepthStencilView* myDepth;
		};

		ID3D11Texture2D* myTexture = nullptr;
		ID3D11ShaderResourceView* mySRV = nullptr;
		D3D11_VIEWPORT* myViewport = nullptr;
	};
}
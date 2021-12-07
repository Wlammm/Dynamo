#pragma once

namespace Dynamo
{
	class Texture;

	class GBuffer
	{
	public:
		enum GBufferTexture
		{
			ALBEDO,
			NORMAL,
			MATERIAL,
			POSITION,
			VERTEXNORMAL,
			AMBIENTOCCLUSION,
			DEPTH,
			COUNT
		};


	public:
		GBuffer() = default;
		~GBuffer() = default;

		void ClearTextures();
		void SetAsActiveTarget(Texture* aDepth = nullptr);
		void SetAsResourceOnSlot(GBufferTexture aTexture, uint aSlot) const;
		void SetAllAsResources();

		void Release();

	private:
		friend class TextureFactory;

		std::array<ID3D11Texture2D*, GBufferTexture::COUNT> myTextures;
		std::array<ID3D11RenderTargetView*, GBufferTexture::COUNT> myRTVs;
		std::array<ID3D11ShaderResourceView*, GBufferTexture::COUNT> mySRVs;
		D3D11_VIEWPORT* myViewport;
	};
}
#pragma once

namespace Dynamo
{
	class RenderUtils
	{
	public:
		enum BlendState
		{
			BLENDSTATE_DISABLE,
			BLENDSTATE_ALPHABLEND,
			BLENDSTATE_FOLLIAGE,
			BLENDSTATE_ADDITIVE,
			BLENDSTATE_COUNT
		};

		enum SamplerState
		{
			SAMPLERSTATE_POINT,
			SAMPLERSTATE_TRILINEARWRAP,
			SAMPLERSTATE_TRILINEARCLAMP,
			SAMPLERSTATE_TRILINEAR,
			SAMPLERSTATE_COUNT
		};

		enum DepthState
		{
			DEPTHSTATE_DEFUALT,
			DEPTHSTATE_READONLY,
			DEPTHSTATE_COUNT
		};

	public:
		static void Create();
		static void Destroy();

		static void SetSamplerState(SamplerState aState, uint aSlot);
		static void SetBlendState(BlendState aState);
		static void SetDepthState(DepthState aState);

		template<typename T>
		static void MapBuffer(T& aBufferData, ID3D11Buffer* aBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			HRESULT result = Main::GetContext()->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			DYN_ASSERT(SUCCEEDED(result) && "Failed to map buffer.");
			memcpy(bufferData.pData, &aBufferData, sizeof(T));
			Main::GetContext()->Unmap(aBuffer, 0);
		}

		template<typename T>
		static void CreateBuffer(ID3D11Buffer*& aBuffer)
		{
			HRESULT result;

			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.ByteWidth = sizeof(T);
			result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &aBuffer);
			DYN_ASSERT(SUCCEEDED(result));
		}

	private:
		static void CreateBlendStates();
		static void CreateSamplerStates();
		static void CreateDepthStates();

	private:
		inline static std::array<ID3D11BlendState*, BLENDSTATE_COUNT> myBlendStates;
		inline static std::array<ID3D11SamplerState*, SAMPLERSTATE_COUNT> mySamplerStates;
		inline static std::array<ID3D11DepthStencilState*, DEPTHSTATE_COUNT> myDepthStates;
	};
}
#pragma once

#include "RenderStructs.hpp"

namespace Dynamo
{
	class MeshRenderer;
	class Shader;

	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer() = default;

		void Render(const CU::DArray<MeshRenderer*>& someModels);

	private:
		void CreateBuffers();

		template<typename T>
		void MapBuffer(T& aBufferData, ID3D11Buffer* aBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			HRESULT result = Main::GetContext()->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			assert(SUCCEEDED(result) && "Failed to map buffer.");
			memcpy(bufferData.pData, &aBufferData, sizeof(T));
			Main::GetContext()->Unmap(aBuffer, 0);
		}

	private:
		Material* myDefaultMaterial = nullptr;
		Shader* myDefaultPixelShader = nullptr;
		Shader* myDefaultVertexShader = nullptr;
		
		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;
		
		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;
	};
}
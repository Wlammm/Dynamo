#pragma once

#include "RenderStructs.hpp"

namespace Dynamo
{
	class MeshRenderer;
	class DirectionalLight;
	class AmbientLight;
	class PointLight;
	class SpotLight;

	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer() = default;

		void GenerateGBuffer(const CU::DArray<MeshRenderer*>& someModels);

		void Render(const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights,
			const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights);

	private:
		void CreateBuffers();
		void CreateShaders();

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
		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;

		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;
		
		DirectionalLightBuffer myDirLightBufferData;
		ID3D11Buffer* myDirLightBuffer = nullptr;
		
		AmbientLightBuffer myAmbLightBufferData;
		ID3D11Buffer* myAmbLightBuffer = nullptr;

		Shader* myFSVertexShader = nullptr;
		Shader* myMeshVertexShader = nullptr;

		Shader* myDirLightShader = nullptr;
		Shader* myAmbLightShader = nullptr;
		Shader* myPointLightShader = nullptr;
		Shader* mySpotLightShader = nullptr;
		Shader* myGBufferShader = nullptr;
	};
}
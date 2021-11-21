#pragma once

#include "RenderStructs.hpp"

namespace Dynamo
{
	class MeshRenderer;
	class Shader;
	class AmbientLight;
	class DirectionalLight;
	class PointLight;
	class SpotLight;

	struct Mesh;

	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		void Render(const CU::DArray<MeshRenderer*>& someModels, const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights,
			const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights);

	private:
		void CreateBuffers();
		void CreateRSStates();

		void MapFrameBuffer(Camera* aCamera);
		void MapObjectBuffer(MeshRenderer* aModel);
		void SetMeshSettings(const Mesh& aMesh);
		void MapDirLightBuffer(const CU::DArray<DirectionalLight*>& someDirLights);
		void MapAmbLightBuffer(const CU::DArray<AmbientLight*>& someAmbLights);
		void MapPointLightBuffer(const CU::DArray<PointLight*>& somePointLights);
		void MapSpotLightBuffer(const CU::DArray<SpotLight*>& someSpotLights);

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

		DirectionalLightBuffer myDirLightBufferData;
		ID3D11Buffer* myDirLightBuffer = nullptr;

		AmbientLightBuffer myAmbLightBufferData;
		ID3D11Buffer* myAmbLightBuffer = nullptr;

		ForwardPointLightBuffer myPointLightBufferData;
		ID3D11Buffer* myPointLightBuffer = nullptr;

		ForwardSpotLightBuffer mySpotLightBufferData;
		ID3D11Buffer* mySpotLightBuffer = nullptr;

		ID3D11RasterizerState* myRSState;
	};
}
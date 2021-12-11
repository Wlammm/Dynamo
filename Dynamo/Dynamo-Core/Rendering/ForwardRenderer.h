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

		std::vector<PointLight*> GetSortedPointLights(const std::vector<PointLight*>& someInData, MeshRenderer* aInstance);
		std::vector<SpotLight*> GetSortedSpotLights(const std::vector<SpotLight*>& someInData, MeshRenderer* aInstance);

		void MapFrameBuffer(Camera* aCamera);
		void MapObjectBuffer(MeshRenderer* aModel);
		void SetMeshSettings(const Mesh& aMesh);

		void MapDirLightBuffer(const std::vector<DirectionalLight*>& someDirLights);
		void MapAmbLightBuffer(const std::vector<AmbientLight*>& someAmbLights);
		void MapPointLightBuffer(const std::vector<PointLight*>& somePointLights);
		void MapSpotLightBuffer(const std::vector<SpotLight*>& someSpotLights);
		void MapEmissiveBuffer();
		void MapMaterialBuffer(Material* aMaterial);

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

		EmissiveBuffer myEmissiveBufferData;
		ID3D11Buffer* myEmissiveBuffer = nullptr;

		MaterialBuffer myMaterialBufferData;
		ID3D11Buffer* myMaterialBuffer = nullptr;

		ID3D11RasterizerState* myRSState;
	};
}
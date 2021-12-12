#pragma once

#include "RenderStructs.hpp"
#include "RenderCommands.hpp"

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

		void Render(const CU::DArray<MeshCommand>& someModels, const CU::DArray<DirectionalLightCommand>& someDirLights, const CU::DArray<AmbientLightCommand>& someAmbLights,
			const CU::DArray<PointLightCommand>& somePointLights, const CU::DArray<SpotLightCommand>& someSpotLights);

	private:
		void CreateBuffers();
		void CreateRSStates();

		std::vector<PointLightCommand> GetSortedPointLights(const std::vector<PointLightCommand>& someInData, const MeshCommand& aInstance);
		std::vector<SpotLightCommand> GetSortedSpotLights(const std::vector<SpotLightCommand>& someInData, const MeshCommand& aInstance);

		void MapFrameBuffer(Camera* aCamera);
		void MapObjectBuffer(const MeshCommand& aMesh);
		void SetMeshSettings(const Mesh& aMesh);

		void MapDirLightBuffer(const std::vector<DirectionalLightCommand>& someDirLights);
		void MapAmbLightBuffer(const std::vector<AmbientLightCommand>& someAmbLights);
		void MapPointLightBuffer(const std::vector<PointLightCommand>& somePointLights);
		void MapSpotLightBuffer(const std::vector<SpotLightCommand>& someSpotLights);
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
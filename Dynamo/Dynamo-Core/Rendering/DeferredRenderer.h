#pragma once

#include "RenderStructs.hpp"
#include "RenderCommands.hpp"

namespace Dynamo
{
	class MeshRenderer;
	class DirectionalLight;
	class AmbientLight;
	class PointLight;
	class SpotLight;
	class BloomEffect;

	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer();

		void GenerateGBuffer(const CU::DArray<MeshCommand>& someModels);

		void Render(const CU::DArray<DirectionalLightCommand>& someDirLights, const CU::DArray<AmbientLightCommand>& someAmbLights,
			const CU::DArray<PointLightCommand>& somePointLights, const CU::DArray<SpotLightCommand>& someSpotLights);

		void DrawRenderPass(const int aPass);

	private:
		void CreateBuffers();
		void CreateShaders();

	private:
		struct PassBuffer 
		{
			int myPass;
			Vec3i padding;
		} myPassBufferData;
		ID3D11Buffer* myPassBuffer = nullptr;

		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;

		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;
		
		DirectionalLightBuffer myDirLightBufferData;
		ID3D11Buffer* myDirLightBuffer = nullptr;
		
		AmbientLightBuffer myAmbLightBufferData;
		ID3D11Buffer* myAmbLightBuffer = nullptr;

		DeferredPointLightBuffer myPointLightBufferData;
		ID3D11Buffer* myPointLightBuffer = nullptr;

		DeferredSpotLightBuffer mySpotLightBufferData;
		ID3D11Buffer* mySpotLightBuffer = nullptr;

		EmissiveBuffer myEmissiveBufferData;
		ID3D11Buffer* myEmissiveBuffer = nullptr;

		MaterialBuffer myMaterialBufferData;
		ID3D11Buffer* myMaterialBuffer = nullptr;

		BoneBuffer myBoneBufferData;
		ID3D11Buffer* myBoneBuffer = nullptr;

		Shader* myFSVertexShader = nullptr;
		Shader* myMeshVertexShader = nullptr;
		Shader* myMaterialPassShader = nullptr;

		Shader* myDirLightShader = nullptr;
		Shader* myAmbLightShader = nullptr;
		Shader* myPointLightShader = nullptr;
		Shader* mySpotLightShader = nullptr;
		Shader* myEmissiveShader = nullptr;
		Shader* myGBufferShader = nullptr;
	};
}
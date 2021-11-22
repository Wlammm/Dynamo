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

	private:
		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;

		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;
		
		DirectionalLightBuffer myDirLightBuffer;
		ID3D11Buffer* myDirLightBuffer = nullptr;
		
		AmbientLightBuffer myAmbLightBuffer;
		ID3D11Buffer* myAmbLightBuffer = nullptr;

		Shader* myVertexShader = nullptr;

		Shader* myDirLightShader = nullptr;
		Shader* myAmbLightShader = nullptr;
		Shader* myPointLightShader = nullptr;
		Shader* mySpotLightShader = nullptr;
		Shader* myGBufferShader = nullptr;
	};
}
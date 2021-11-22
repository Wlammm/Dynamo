#include "pch.h"
#include "DeferredRenderer.h"

namespace Dynamo
{
	DeferredRenderer::DeferredRenderer()
	{
		CreateBuffers();
		CreateShaders();
	}

	void DeferredRenderer::GenerateGBuffer(const CU::DArray<MeshRenderer*>& someModels)
	{
	}

	void DeferredRenderer::Render(const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights, const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights)
	{
	}

	void DeferredRenderer::CreateBuffers()
	{
		HRESULT result;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDesc.ByteWidth = sizeof(FrameBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myFrameBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(ObjectBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myObjectBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(DirectionalLightBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myDirLightBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(AmbientLightBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myAmbLightBuffer);
		assert(SUCCEEDED(result));
	}

	void DeferredRenderer::CreateShaders()
	{
		myVertexShader = ShaderFactory::GetShader("Shaders/FullscreenVS.cso", ShaderType::VertexShader);

		myGBufferShader = ShaderFactory::GetShader("Shaders/GBuffer.cso", ShaderType::PixelShader);
		myDirLightShader = ShaderFactory::GetShader("Shaders/DirectionalLightShader.cso", ShaderType::PixelShader);
		myAmbLightShader = ShaderFactory::GetShader("Shaders/AmbientLightShader.cso", ShaderType::PixelShader);
	}
}

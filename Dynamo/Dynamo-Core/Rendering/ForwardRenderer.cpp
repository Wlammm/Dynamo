#include "pch.h"
#include "ForwardRenderer.h"

namespace Dynamo
{
	ForwardRenderer::ForwardRenderer()
	{
		myDefaultPixelShader = ShaderFactory::GetShader("Shaders/ForwardPS.cso", ShaderType::PixelShader);
		myDefaultVertexShader = ShaderFactory::GetShader("Shaders/VertexShader.cso", ShaderType::VertexShader);

		CreateBuffers();
	}

	void ForwardRenderer::Render(const CU::DArray<MeshRenderer*>& someModels)
	{
		HRESULT result;

		D3D11_MAPPED_SUBRESOURCE bufferData;

	}

	void ForwardRenderer::CreateBuffers()
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
	}
}
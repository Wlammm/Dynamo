#include "pch.h"
#include "FullscreenRenderer.h"

namespace Dynamo
{
	FullscreenRenderer::FullscreenRenderer()
	{
		myVertexShader = ShaderFactory::GetShader("Shaders/FullscreenVS.cso", ShaderType::VertexShader);
		myCopyShader = ShaderFactory::GetShader("Shaders/FullscreenPS-Copy.cso", ShaderType::PixelShader);
	}

	void FullscreenRenderer::Render(Shader* aShader)
	{
		Main::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Main::GetContext()->IASetInputLayout(nullptr);
		Main::GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		Main::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		myVertexShader->Bind();
		aShader->Bind();
		Main::GetContext()->Draw(3, 0);
	}

	void FullscreenRenderer::RenderCopy()
	{
		Render(myCopyShader);
	}
}
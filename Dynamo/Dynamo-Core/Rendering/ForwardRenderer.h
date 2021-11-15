#pragma once

#include "RenderStructs.hpp"

namespace Dynamo
{
	class MeshRenderer;

	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer() = default;

		void Render(const CU::DArray<MeshRenderer*>& someModels);

	private:
		void CreateBuffers();

	private:
		Shader* myDefaultPixelShader = nullptr;
		Shader* myDefaultVertexShader = nullptr;

		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;

		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;
	};
}
#pragma once

#include "pch.h"
#include "Shader.h"

namespace Dynamo
{
	struct Material
	{
		ID3D11ShaderResourceView* myAlbedo = nullptr;
		ID3D11ShaderResourceView* myNormal = nullptr;
		ID3D11ShaderResourceView* myMaterial = nullptr;

		Shader* myPixelShader = nullptr;
		Shader* myVertexShader = nullptr;

		std::array<ID3D11ShaderResourceView*, 4> myCustomTextures = { nullptr, nullptr, nullptr, nullptr };
	};
}
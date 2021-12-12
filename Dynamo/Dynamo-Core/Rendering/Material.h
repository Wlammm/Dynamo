#pragma once

#include "pch.h"
#include "Shader.h"

namespace Dynamo
{
	enum class SurfaceType
	{
		Opaque,
		Transparent
	};

	struct Material
	{
		ID3D11ShaderResourceView* myAlbedo = nullptr;
		ID3D11ShaderResourceView* myNormal = nullptr;
		ID3D11ShaderResourceView* myMaterial = nullptr;

		Shader* myPixelShader = nullptr;
		Shader* myVertexShader = nullptr;

		std::array<ID3D11ShaderResourceView*, 4> myCustomTextures = { nullptr, nullptr, nullptr, nullptr };

		float myRoughnessConstant = 0;
		float myRoughnessInterpolation = 1;

		float myMetalnessConstant = 0;
		float myMetalnessInterpolation = 1;

		bool myReceiveShadows = true;
		SurfaceType mySurfaceType = SurfaceType::Opaque;
	};
}
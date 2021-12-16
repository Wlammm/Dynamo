#pragma once

#include "pch.h"
#include "Shader.h"

namespace Dynamo
{
	enum class Renderer
	{
		Deferred,
		Forward
	};

	struct Material
	{
		Dyn::SRV* myAlbedo = nullptr;
		Dyn::SRV* myNormal = nullptr;
		Dyn::SRV* myMaterial = nullptr;

		Shader* myPixelShader = nullptr;
		Shader* myVertexShader = nullptr;

		std::array<Dyn::SRV*, 4> myCustomTextures = { nullptr, nullptr, nullptr, nullptr };
		std::array<float, 4> myCustomValues = { 0, 0, 0, 0 };

		float myRoughnessConstant = 0;
		float myRoughnessInterpolation = 1;

		float myMetalnessConstant = 0;
		float myMetalnessInterpolation = 1;

		bool myReceiveShadows = true;
		Renderer myRenderer = Renderer::Deferred;

		bool myIsDepthTested = true;

		std::filesystem::path myMaterialPath = "";
	};
}
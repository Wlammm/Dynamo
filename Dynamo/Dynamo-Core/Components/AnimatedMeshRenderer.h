#pragma once

#include "MeshRenderer.h"

namespace Dynamo
{
	struct Animation;

	class AnimatedMeshRenderer : public MeshRenderer
	{
		COMP(AnimatedMeshRenderer)
	public:
		AnimatedMeshRenderer();

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void Update() override;

	private:
		Animation* myAnimation = nullptr;
	};
}
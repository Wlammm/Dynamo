#pragma once

#include "MeshRenderer.h"

namespace Dynamo
{
	struct Animation;

	class Animator : public Component
	{
		COMP(Animator)
	public:
		Animator();

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void OnCreate() override;

		void Update() override;
		void EditorUpdate() override;

	private:
		MeshRenderer* myMeshRenderer;

		Animation* myAnimation = nullptr;
		std::array<Mat4f, 128> myBoneTransforms;

		uint myCurrentFrameIndex = 1;
		float myAnimationProgress = 0;
	};
}
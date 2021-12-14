#pragma once

#include "MeshRenderer.h"

namespace Dynamo
{
	class AnimatedMeshRenderer : public MeshRenderer
	{
		COMP(AnimatedMeshRenderer)
	public:
		AnimatedMeshRenderer();

		void Update() override;
	};
}
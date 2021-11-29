#pragma once

#include <Scene/Scene.h>

namespace Editor
{
	class Scene : public Dyn::Scene
	{
	public:
		Scene();

		void Update() override;
	};
}
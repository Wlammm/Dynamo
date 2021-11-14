#pragma once

#include <Scene/Scene.h>

class GameScene : public Dyn::Scene
{
public:
	GameScene();

	void Update() override;
};
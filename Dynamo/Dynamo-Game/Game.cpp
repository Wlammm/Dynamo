#include "GamePch.h"
#include "Game.h"
#include "GameScene.h"

#include "Scene/SceneSerializer.h"

Game::Game()
{
	Dyn::Main::SetScene(new GameScene());

	Dyn::SceneSerializer::LoadScene(Dyn::Main::GetScene(), "Assets/Scenes/DefaultScene.dynscene");
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

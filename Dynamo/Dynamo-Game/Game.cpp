#include "GamePch.h"
#include "Game.h"
#include "GameScene.h"

Game::Game()
{
	Dyn::Main::SetScene(myScene.get());
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

#include "GamePch.h"
#include "GameScene.h"

GameScene::GameScene()
{
	InternalInit();
}

void GameScene::Update()
{
	GetComponentAdmin().Update();
	GetComponentAdmin().LateUpdate();
}
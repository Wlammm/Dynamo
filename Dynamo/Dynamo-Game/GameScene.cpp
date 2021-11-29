#include "GamePch.h"
#include "GameScene.h"

GameScene::GameScene()
{
	InternalInit();
}

void GameScene::Update()
{
	GetComponentAdmin().Update(Time::GetDeltaTime());
	GetComponentAdmin().UpdateComponents();
	GetComponentAdmin().LateUpdateComponents();
}

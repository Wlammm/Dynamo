#include "GamePch.h"
#include "Game.h"
#include "GameScene.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"

Game::Game()
{
	Dyn::Main::SetScene(myScene.get());

	GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::Camera* cameraComp = camera->AddComponent<Dyn::Camera>();
	camera->SetTag(Tag::MainCamera);

	GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
	meshComp->SetModel("Assets/Fbx/Particle_Chest.fbx");
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

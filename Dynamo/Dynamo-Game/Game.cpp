#include "GamePch.h"
#include "Game.h"
#include "GameScene.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"

Game::Game()
{
	Dyn::Main::SetScene(myScene.get());

	GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::Camera* camComp = camera->AddComponent<Dyn::Camera>();
	camComp->EnableFreeCamera();
	camera->SetTag(Tag::MainCamera);
	camera->GetTransform().SetPosition({ 0, 0, -300 });

	GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
	meshComp->SetModel("Assets/Fbx/Particle_Chest.fbx");
	meshComp->ApplyModelMaterial();
	//mesh->GetTransform().SetScale({ 0.5f, 0.5f, 0.5f });
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

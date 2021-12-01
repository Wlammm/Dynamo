#include "GamePch.h"
#include "Game.h"
#include "GameScene.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Components/DirectionalLight.h"
#include "Components/AmbientLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

#include "Rendering/FullscreenEffects/BloomEffect.h"

Game::Game()
{
	Dyn::Main::GetRenderManager().AddFullscreenEffect(new Dyn::BloomEffect(), 0);
	Dyn::Main::GetRenderManager().GetFullscreenEffect<Dyn::BloomEffect>()->SetCutoff(10);

	Dyn::Main::SetScene(new GameScene());

	GameObject* defaultLights = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::DirectionalLight* dirLight = defaultLights->AddComponent<Dyn::DirectionalLight>();
	dirLight->SetIntensity(10.0f);
	defaultLights->GetTransform().SetRotationDeg({ -45, -45, -45 });
	Dyn::AmbientLight* ambLight = defaultLights->AddComponent<Dyn::AmbientLight>();
	ambLight->SetIntensity(10.f);
	
	//GameObject* pointLight = Dyn::Main::GetScene()->CreateGameObject();
	//Dyn::PointLight* pLight = pointLight->AddComponent<Dyn::PointLight>();
	//pLight->SetIntensity(500000);
	//pLight->SetRange(10000);
	//pLight->SetColor({ 1, 0.1f, 0.1f, 1 });
	//pointLight->GetTransform().SetPosition({ 0, 100, 0 });

	//GameObject* spotLight = Dyn::Main::GetScene()->CreateGameObject();
	//Dyn::SpotLight* sLight = spotLight->AddComponent<Dyn::SpotLight>();
	//sLight->SetRange(10000);
	//sLight->SetIntensity(1000000);
	//sLight->SetColor({ 0, 1, 0, 1 });
	//spotLight->GetTransform().SetRotationDeg({0, 0, 0});
	//spotLight->GetTransform().SetPosition({ 0, 0, -1000 });
	//
	//GameObject* spotLight1 = Dyn::Main::GetScene()->CreateGameObject();
	//Dyn::SpotLight* sLight1 = spotLight1->AddComponent<Dyn::SpotLight>();
	//sLight1->SetRange(10000);
	//sLight1->SetIntensity(1000000);
	//sLight1->SetColor({ 1, 0, 0, 1 });
	//spotLight1->GetTransform().SetRotationDeg({ 0, 1, 0 });
	//spotLight1->GetTransform().SetPosition({ 250, 0, -1000 });

	GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::Camera* camComp = camera->AddComponent<Dyn::Camera>();
	camComp->EnableFreeCamera();
	camera->SetTag(Tag::MainCamera);
	camera->GetTransform().SetPosition({ 0, 0, -300 });

	GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
	meshComp->SetModel("Assets/Fbx/Chest.fbx");
	meshComp->ApplyModelMaterial();
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

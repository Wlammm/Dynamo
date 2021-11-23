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
	//Dyn::Main::GetRenderManager().AddFullscreenEffect(new Dyn::BloomEffect());

	Dyn::Main::SetScene(myScene.get());
	GameObject* defaultLights = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::DirectionalLight* dirLight = defaultLights->AddComponent<Dyn::DirectionalLight>();
	dirLight->SetIntensity(1);
	defaultLights->GetTransform().SetRotationDeg({ 45, 45, 45 });
	Dyn::AmbientLight* ambLight = defaultLights->AddComponent<Dyn::AmbientLight>();
	ambLight->SetIntensity(1.f);

	GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::Camera* camComp = camera->AddComponent<Dyn::Camera>();
	camComp->EnableFreeCamera();
	camera->SetTag(Tag::MainCamera);
	camera->GetTransform().SetPosition({ 0, 0, -300 });

	GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
	Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
	meshComp->SetModel("Assets/Fbx/Particle_Chest.fbx");
	meshComp->ApplyModelMaterial();
}

Game::~Game()
{
	Dyn::Main::SetScene(nullptr);
}

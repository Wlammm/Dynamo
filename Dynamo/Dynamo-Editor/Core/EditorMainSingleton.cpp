#include "EditorPch.h"
#include "Core/EditorMainSingleton.h"

#include "Managers/EditorManager.h"
#include "PostMaster/EditorPostMaster.h"

// TEMP (Remove when not initializing scene objects in InitScene().)
#include "Scene/EditorScene.h"
#include <Components/DirectionalLight.h>
#include <Components/AmbientLight.h>
#include <Components/Camera.h>
#include <Components/MeshRenderer.h>
#include <Components/PointLight.h>
#include <Components/SpotLight.h>

#include "Scene/SceneSerializer.h"

namespace Editor
{
	void MainSingleton::Init()
	{
		assert(ourInstance == nullptr && "Singleton already exists.");
		ourInstance = new MainSingleton();

		ourInstance->myEditorManager = new EditorManager();
		ourInstance->myEditorManager->Init();

		ourInstance->myPostMaster = new PostMaster();

		InitScenes();
	}

	void MainSingleton::Destroy()
	{
		delete ourInstance->myEditorManager;
		delete ourInstance->myPostMaster;

		delete ourInstance;
		ourInstance = nullptr;
	}

	void MainSingleton::Update()
	{
		ourInstance->myEditorManager->Update();
	}

	void MainSingleton::SetSelectedGameObject(GameObject* aGameObject)
	{
		ourInstance->mySelectedGameObject = aGameObject;
	}

	GameObject* MainSingleton::GetSelectedGameObject()
	{
		return ourInstance->mySelectedGameObject;
	}

	PostMaster* MainSingleton::GetPostMaster()
	{
		return ourInstance->myPostMaster;
	}

	EditorManager* MainSingleton::GetEditorManager()
	{
		return ourInstance->myEditorManager;
	}

	void MainSingleton::InitScenes()
	{
		Dyn::Main::SetScene(new Scene());
		//
		//GameObject* defaultLights = Dyn::Main::GetScene()->CreateGameObject();
		//Dyn::DirectionalLight* dirLight = defaultLights->AddComponent<Dyn::DirectionalLight>();
		//dirLight->SetIntensity(10.0f);
		//defaultLights->GetTransform().SetRotationDeg({ -45, -45, -45 });
		//Dyn::AmbientLight* ambLight = defaultLights->AddComponent<Dyn::AmbientLight>();
		//ambLight->SetIntensity(10.f);
		//
		//GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
		//Dyn::Camera* camComp = camera->AddComponent<Dyn::Camera>();
		//camComp->EnableFreeCamera();
		//camera->SetTag(Tag::MainCamera);
		//camera->GetTransform().SetPosition({ 0, 0, -300 });
		//
		//GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
		//mesh->GetTransform().SetScale(3, 1, 3);
		//mesh->GetTransform().SetRotationDeg(-90, 0, 0);
		//Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
		//meshComp->SetModel("Assets/Fbx/DefaultAssets/Plane.fbx");
		//meshComp->ApplyModelMaterial();
		//
		//GameObject* chest = Dyn::Main::GetScene()->CreateGameObject();
		//chest->GetTransform().SetRotationDeg(0, 0, 0);
		//Dyn::MeshRenderer* chestComp = chest->AddComponent<Dyn::MeshRenderer>();
		//chestComp->SetModel("Assets/Fbx/DefaultAssets/Chest.fbx");
		//chestComp->ApplyModelMaterial();
		//
		//GameObject* pointLight = Dyn::Main::GetScene()->CreateGameObject();
		//Dyn::PointLight* light = pointLight->AddComponent<Dyn::PointLight>();
		//light->SetColor({ 1, 0, 0, 1 });
		//
		//Dyn::Main::GetScene()->SetSavePath("Assets/Scenes/DefaultScene.dynscene");
		//
		//Dyn::SceneSerializer::SaveScene(Dyn::Main::GetScene(), "Assets/Scenes/DefaultScene.dynscene");

		Dyn::SceneSerializer::LoadScene(Dyn::Main::GetScene(), "Assets/Scenes/DefaultScene.dynscene");
	}
}
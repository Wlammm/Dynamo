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

		GameObject* defaultLights = Dyn::Main::GetScene()->CreateGameObject();
		Dyn::DirectionalLight* dirLight = defaultLights->AddComponent<Dyn::DirectionalLight>();
		dirLight->SetIntensity(10.0f);
		defaultLights->GetTransform().SetRotationDeg({ -45, -45, -45 });
		Dyn::AmbientLight* ambLight = defaultLights->AddComponent<Dyn::AmbientLight>();
		ambLight->SetIntensity(10.f);
		
		GameObject* camera = Dyn::Main::GetScene()->CreateGameObject();
		Dyn::Camera* camComp = camera->AddComponent<Dyn::Camera>();
		camComp->EnableFreeCamera();
		camera->SetTag(Tag::MainCamera);
		camera->GetTransform().SetPosition({ 0, 0, -300 });
		
		GameObject* mesh = Dyn::Main::GetScene()->CreateGameObject();
		Dyn::MeshRenderer* meshComp = mesh->AddComponent<Dyn::MeshRenderer>();
		meshComp->SetModel("Assets/Fbx/Chest.fbx");
		meshComp->ApplyModelMaterial();
		
		Dyn::SceneSerializer::SaveScene(Dyn::Main::GetScene(), "Assets/Scenes/DefaultScene.dynscene");

		//Dyn::SceneSerializer::LoadScene(Dyn::Main::GetScene(), "Assets/Scenes/DefaultScene.dynscene");
	}
}
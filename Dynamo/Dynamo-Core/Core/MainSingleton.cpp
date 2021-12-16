#include "pch.h"
#include "MainSingleton.h"

#include "WindowHandler.h"
#include "DirectXFramework.h"

#include "Scene/Scene.h"
#include"Rendering/RenderManager.h"

#include "Components/Camera.h"

#include "Utils/ImGuiManager.h"

#include "Rendering/FullscreenEffects/HDREffect.h"

namespace Dynamo
{
	MainSingleton* MainSingleton::ourInstance = nullptr;

	void MainSingleton::Init()
	{
		assert(ourInstance == nullptr && "MainSingleton already initialized.");
		ourInstance = new MainSingleton();

		Console::Create();
		Input::Create();
		Time::Create();

		// These needs to be in order. 
		// 1. WindowHandler
		// 2. DirectXFramework
		ourInstance->myWindowHandler = new WindowHandler();
		ourInstance->myFramework = new DirectXFramework();
		ourInstance->myRenderManager = new RenderManager();

		ImGuiManager::Start();
	}

	void MainSingleton::Destroy()
	{
		assert(ourInstance != nullptr && "MainSingleton already destroyed.");

		ImGuiManager::Destroy();
		Time::Destroy();
		Input::Destroy();
		Console::Destroy();

		delete ourInstance->myRenderManager;
		delete ourInstance->myFramework;
		delete ourInstance->myWindowHandler;
	}

	void MainSingleton::Update()
	{
		Time::Update();
		GetRenderManager().Update();

		if (Input::IsKeyDown(KeyCode::Y))
		{
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Info, 3000, "Hej jag en ankadsadsadsadsakjldsakldsajksaödsjkalödjsaklödjskalöjköl\nfjsdklfjdksfjdsklfjdasköljdsaföl"));
		}

		if (GetScene() != nullptr)
		{
			GetScene()->Update();
		}

		Debug::Update();
	}

	const Vec2ui& MainSingleton::GetWindowSize()
	{
		return ourInstance->myWindowSize;
	}
	
	const Vec2ui& MainSingleton::GetWindowOffset()
	{
		return ourInstance->myWindowOffset;
	}

	const Vec2ui& MainSingleton::GetWindowResolution()
	{
		assert(ourInstance->myWindowResolution.x != 0 && "WindowHandler not initialized yet.");
		return ourInstance->myWindowResolution;
	}
	
	const Color& MainSingleton::GetClearColor()
	{
		return ourInstance->myClearColor;
	}
	
	const std::wstring& MainSingleton::GetWindowTitle()
	{
		return ourInstance->myWindowTitle;
	}

	const HWND& MainSingleton::GetHWND()
	{
		return ourInstance->myWindowHandle;
	}

	const bool& MainSingleton::GetEngineRunning()
	{
		return ourInstance->myEngineRunning;
	}

	void MainSingleton::SetEngineRunning(const bool& aValue)
	{
		ourInstance->myEngineRunning = aValue;
	}
	
	ID3D11Device* MainSingleton::GetDevice()
	{
		assert(ourInstance->myDevice != nullptr && "DirectXFramework not initialized.");
		return ourInstance->myDevice;
	}

	ID3D11DeviceContext* MainSingleton::GetContext()
	{
		assert(ourInstance->myContext != nullptr && "DirectXFramework not initialized.");
		return ourInstance->myContext;
	}

	WindowHandler* MainSingleton::GetWindowHandler()
	{
		return ourInstance->myWindowHandler;
	}

	DirectXFramework* MainSingleton::GetFramework()
	{
		return ourInstance->myFramework;
	}

	Scene* MainSingleton::GetScene()
	{
		return ourInstance->myScene;
	}

	void MainSingleton::SetScene(Scene* aScene)
	{
		if (ourInstance->myScene && ourInstance->myScene != aScene)
		{
			delete ourInstance->myScene;
		}

		ourInstance->myScene = aScene;
	}

	void MainSingleton::SetOverrideCamera(Camera* aCamera)
	{
		ourInstance->myOverrideCamera = aCamera;
	}

	Camera* MainSingleton::GetMainCamera()
	{
		if (ourInstance->myOverrideCamera)
			return ourInstance->myOverrideCamera;

		if (ourInstance->myMainCamera && ourInstance->myMainCamera->IsValid())
			return ourInstance->myMainCamera;

		if (!ourInstance->myScene)
			return nullptr;

		std::vector<GameObject*> cameras = GetScene()->GetComponentAdmin().GetGameObjectsWithComponent<Camera>();
		for (const auto& cam : cameras)
		{
			if (cam->GetTag() == Tag::MainCamera)
			{
				ourInstance->myMainCamera = cam->GetComponent<Camera>();
				return ourInstance->myMainCamera;
			}
		}

		return nullptr;
	}

	RenderManager& MainSingleton::GetRenderManager()
	{
		return *ourInstance->myRenderManager;
	}
}
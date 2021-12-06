#include "EditorPch.h"
#include "EditorManager.h"

#include "Entry.h"

#include "Core/EditorWindow.h"
#include "Scene/EditorScene.h"
#include "Utils/ThemeUtils.h"

#include "Windows/Viewport.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/ContentBrowser.h"

#include "Systems/Toolbar.h"
#include "Systems/Guizmos.h"
#include "Systems/SceneSaving.h"

/*
* __pragma(warning(suppress: XXXX))
*/
namespace Editor
{
	EditorManager::EditorManager()
	{
		Themes::ApplyDefaultTheme();

		CreateWindows();
		CreateSystems();
	}
	
	EditorManager::~EditorManager()
	{
		for (int i = 0; i < myWindows.sizeI(); ++i)
		{
			delete myWindows[i];
			myWindows[i] = nullptr;
		}
		myWindows.clear();

		for (int i = 0; i < mySystems.sizeI(); ++i)
		{
			delete mySystems[i];
			mySystems[i] = nullptr;
		}
		mySystems.clear();
	}

	void EditorManager::Init()
	{
		InitWindows();
		InitSystems();
	}

	void EditorManager::CreateWindows()
	{
		AddWindow(new Viewport());
		AddWindow(new Hierarchy());
		AddWindow(new Inspector());
		AddWindow(new ContentBrowser());
	}

	void EditorManager::CreateSystems()
	{
		AddSystem(new Toolbar());
		AddSystem(new Guizmos());
		AddSystem(new SceneSaving());
	}

	void EditorManager::InitWindows()
	{
		for (uint i = 0; i < myWindows.size(); ++i)
		{
			myWindows[i]->Init();
		}
	}

	void EditorManager::InitSystems()
	{
		for (uint i = 0; i < mySystems.size(); ++i)
		{
			mySystems[i]->Init();
		}
	}
	
	void EditorManager::Update()
	{
		BeginImGuiDocking();

		for (int i = 0; i < myWindows.sizeI(); ++i)
		{
			myWindows[i]->DoUpdate();
		}

		for (int i = 0; i < mySystems.sizeI(); ++i)
		{
			mySystems[i]->Update();
		}

		ImGui::End();
	}

	EditorWindow* EditorManager::AddWindow(EditorWindow* aWindow)
	{
		myWindows.Add(aWindow);
		aWindow->myWindowID = myNextID++;
		aWindow->myWindowName = aWindow->myWindowName + "##" + std::to_string(aWindow->myWindowID);
		return aWindow;
	}

	void EditorManager::RemoveWindow(EditorWindow* aWindow)
	{
		myWindows.Remove(aWindow);
		delete aWindow;
	}

	EditorSystem* EditorManager::AddSystem(EditorSystem* aSystem)
	{
		mySystems.Add(aSystem);
		return aSystem;
	}

	void EditorManager::RemoveSystem(EditorSystem* aSystem)
	{
		mySystems.Remove(aSystem);
		delete aSystem;
	}

	void EditorManager::BeginImGuiDocking()
	{
		static bool p_open = true;

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

		ImGui::Begin("EditorInterface", &p_open, window_flags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}
}
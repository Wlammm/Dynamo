#include "EditorPch.h"
#include "SceneSaving.h"
#include "Scene/SceneSerializer.h"

namespace Editor
{
	SceneSaving::SceneSaving()
	{

	}

	void SceneSaving::Update()
	{
		if (Input::IsKeyDown(KeyCode::S) && Input::IsKeyPressed(KeyCode::LeftControl))
		{
			Dyn::Scene* scene = Dyn::Main::GetScene();

			if (scene)
			{
				Dyn::SceneSerializer::SaveScene(scene, scene->GetSavePath());
				Console::Log("Scene saved.");
			}
		}
	}
}
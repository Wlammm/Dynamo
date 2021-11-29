#include "EditorPch.h"
#include "EditorScene.h"

namespace Editor
{
	Scene::Scene()
	{
		InternalInit();
	}

	void Scene::Update()
	{
		GetComponentAdmin().Update(Time::GetDeltaTime());
		GetComponentAdmin().EditorUpdateComponents();
	}
}

#include "EditorPch.h"
#include "Hierarchy.h"

namespace Editor
{
	Hierarchy::Hierarchy()
		: EditorWindow("Hierarchy")
	{ }

	void Hierarchy::Update()
	{
		const std::vector<GameObject*>& gameObjectsInScene = Dyn::Main::GetScene()->GetAllGameObjects();

		for (int i = 0; i < gameObjectsInScene.size(); ++i)
		{
			GameObject* ob = gameObjectsInScene[i];

			std::string nameID = ob->GetName() + "##" + std::to_string(i);
			if (ImGui::Selectable(nameID.c_str(), ob == Main::GetSelectedGameObject()))
			{
				Main::SetSelectedGameObject(ob);
				Main::GetPostMaster()->PostCommand(PostCommand(CommandType::GameObjectSelected, ob));
			}
		}
	}
}

#include "EditorPch.h"
#include "Hierarchy.h"

#include <Components/Camera.h>
#include "Inspector.h"

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
				Main::GetEditorManager()->GetWindow<Inspector>()->SetFocus();
				Main::SetSelectedGameObject(ob);
				Main::GetPostMaster()->PostCommand(PostCommand(CommandType::GameObjectSelected, ob));
			}
		}

		if (ImGui::IsWindowHovered() && Input::IsKeyDown(MouseButton::Right))
		{
			ImGui::OpenPopup("rightclickpopup##hierarchy");
		}

		if (ImGui::IsWindowFocused())
		{
			UpdateHotkeys();
		}

		UpdatePopups();
	}

	void Hierarchy::UpdateHotkeys()
	{
		GameObject* selectedObject = Main::GetSelectedGameObject();

		if (Input::IsKeyDown(KeyCode::Delete) && selectedObject)
		{
			GameObject::Destroy(selectedObject);
			Main::SetSelectedGameObject(nullptr);
		}
	}
	
	void Hierarchy::UpdatePopups()
	{
		if (ImGui::BeginPopup("rightclickpopup##hierarchy"))
		{
			ImGui::TextDisabled("GameObjects");
			ImGui::Separator();

			if(ImGui::Selectable("Create GameObject##hierarchy"))
			{
				CreateNewGameObject();
			}
			ImGui::EndPopup();
		}
	}

	void Hierarchy::CreateNewGameObject()
	{
		GameObject* newObject = Dyn::Main::GetScene()->CreateGameObject();

		Dyn::Camera* cam = Dyn::Main::GetMainCamera();
		if (!cam)
			return;

		Vec3f pos = cam->GetTransform()->GetPosition() + cam->GetTransform()->GetForward() * 250.0f;
		newObject->GetTransform().SetPosition(pos);

		Main::SetSelectedGameObject(newObject);
	}
}
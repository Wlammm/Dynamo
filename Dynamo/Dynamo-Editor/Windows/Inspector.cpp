#include "EditorPch.h"
#include "Inspector.h"

#include "Components/Transform.h"

namespace Editor
{
	Inspector::Inspector()
		: EditorWindow("Inspector")
	{ }

	void Inspector::Update()
	{
		GameObject* selectedObject = Main::GetSelectedGameObject();
		if (!selectedObject)
		{
			ImGui::Text("Select an object to view details.");
			return;
		}

		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanFullWidth;
		if (ImGui::TreeNodeEx("Transform", treeFlags))
		{
			selectedObject->GetComponent<Dyn::Transform>()->ExposeValues();
			ImGui::TreePop();
		}

		auto components = selectedObject->GetAllComponents();
		for (auto comp : components)
		{
			if (dynamic_cast<Dyn::Transform*>(comp))
				continue;

			if (ImGui::TreeNodeEx(comp->GetName().c_str(), treeFlags))
			{
				comp->ExposeValues();
				ImGui::TreePop();
			}
		}

		AddComponentButton();
	}

	void Inspector::AddComponentButton()
	{
		float buttonWidth = ImGui::GetWindowSize().x * 0.9f;
		float remainingWidth = ImGui::GetWindowSize().x - buttonWidth;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + remainingWidth * 0.4f);
		if (ImGui::Button("Add Component", ImVec2(buttonWidth, 40)))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{

		}
	}
}

#include "EditorPch.h"
#include "Inspector.h"

#include "Components/Transform.h"

namespace Editor
{
	Inspector::Inspector()
		: EditorWindow("Inspector")
	{ }

	void Inspector::Init()
	{
		InitCategorizedComponents();
	}

	void Inspector::Update()
	{
		GameObject* selectedObject = Main::GetSelectedGameObject();
		if (!selectedObject)
		{
			ImGui::Text("Select an object to view details.");
			return;
		}

		UpdateHotkeys();

		bool isActive = selectedObject->IsActive();
		if (ImGui::Checkbox("##inspectoractiveobject", &isActive))
		{
			selectedObject->SetActive(isActive);
		}
		ImGui::SameLine();

		ImGui::Text("Name: ");
		ImGui::SameLine();
		std::string objectName = selectedObject->GetName();
		if(ImGui::InputText("##inspectorobjectname", &objectName))
		{
			selectedObject->SetName(objectName);
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform##inspector"))
		{
			selectedObject->GetComponent<Dyn::Transform>()->ExposeValues();
		}

		auto components = selectedObject->GetAllComponents();
		for (auto comp : components)
		{
			if (dynamic_cast<Dyn::Transform*>(comp))
				continue;

			bool isOpen = ImGui::CollapsingHeader(comp->GetName().c_str());
			std::string popupID = "rightclickComponent##" + comp->GetName();
			
			if (ImGui::IsItemHovered() && Input::IsKeyDown(MouseButton::Right))
			{
				ImGui::OpenPopup(popupID.c_str());
			}

			if (isOpen)
			{
				comp->ExposeValues();
			}

			if (ImGui::BeginPopup(popupID.c_str()))
			{
				ImGui::TextDisabled(comp->GetName().c_str());
				ImGui::Separator();
				if (ImGui::Selectable("Remove"))
				{
					Dyn::Main::GetScene()->GetComponentAdmin().RemoveComponentWithTypeID(comp->GetTypeID(), Main::GetSelectedGameObject()->GetGameObjectID());
				}

				ImGui::EndPopup();
			}
		}

		AddComponentButton();
	}

	void Inspector::AddComponentButton()
	{
		static std::string searchField = "";

		float buttonWidth = ImGui::GetWindowSize().x * 0.9f;
		float remainingWidth = ImGui::GetWindowSize().x - buttonWidth;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + remainingWidth * 0.4f);
		bool myOpenedThisFrame = false;
		if (ImGui::Button("Add Component", ImVec2(buttonWidth, 40)))
		{
			searchField = "";
			myOpenedThisFrame = true;
			ImGui::OpenPopup("AddComponent");
		}
		
		ImGui::SetNextWindowSize(ImVec2(buttonWidth, 400));
		ImVec2 popupPos = ImGui::GetCursorScreenPos();
		popupPos.x += remainingWidth * 0.4f;
		ImGui::SetNextWindowPos(popupPos);
		if (ImGui::BeginPopup("AddComponent"))
		{
			ImGui::PushItemWidth(buttonWidth);
			ImGui::InputText("##addComponentFilter", &searchField);
			if (myOpenedThisFrame)
				ImGui::SetKeyboardFocusHere();

			ImGui::Separator();

			for (auto entry : myCategorizedComponents)
			{
				if (ImGui::CollapsingHeader(entry.first.c_str()))
				{
					for (auto type : entry.second)
					{
						if (ImGui::Selectable(type->GetName().c_str()))
						{
							Dyn::Main::GetScene()->GetComponentAdmin().AddComponentWithTypeID(type->GetTypeID(), Main::GetSelectedGameObject()->GetGameObjectID());
						}
					}
				}
			}

			ImGui::EndPopup();
		}
	}

	void Inspector::UpdateHotkeys()
	{
		if (Input::IsKeyDown(KeyCode::Delete))
		{
			GameObject::Destroy(Main::GetSelectedGameObject(), 0);
			Main::SetSelectedGameObject(nullptr);
		}
	}

	void Inspector::InitCategorizedComponents()
	{
		auto compTypes = ComponentRegistry::GetComponentTypes();

		for (auto comp : compTypes)
		{
			myCategorizedComponents[comp->GetCategory()].push_back(comp);
		}
	}
}
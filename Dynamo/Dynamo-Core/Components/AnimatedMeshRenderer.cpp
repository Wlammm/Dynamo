#include "pch.h"
#include "AnimatedMeshRenderer.h"
#include "Rendering/Model.h"
#include "Rendering/Animation.h"

namespace Dynamo
{
	AnimatedMeshRenderer::AnimatedMeshRenderer()
	{

	}

	void AnimatedMeshRenderer::ExposeValues()
	{
		ImGui::Columns(2, 0, false);
		ImGui::Text("Animation");
		ImGui::NextColumn();

		std::string buttonName = "Unassigned";
		if (myAnimation)
			buttonName = myAnimation->myPath.string();
		buttonName += "##animationanimatedmeshrenderer";

		ImGui::Button(buttonName.c_str(), ImVec2(340, 20));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".fbx"))
			{
				std::string path = *(std::string*)payload->Data;
				myAnimation = AnimationFactory::GetAnimation(path);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::NextColumn();

		MeshRenderer::ExposeValues();
	}

	nlohmann::json AnimatedMeshRenderer::Save()
	{
		nlohmann::json json = MeshRenderer::Save();
		json["Animation"] = "";
		if(myAnimation)
			json["Animation"] = myAnimation->myPath.string();
		return json;
	}

	void AnimatedMeshRenderer::Load(nlohmann::json& aJson)
	{
		MeshRenderer::Load(aJson);
		std::string animationPath = aJson["Animation"];
		if (animationPath != "")
			myAnimation = AnimationFactory::GetAnimation(animationPath);
	}

	void AnimatedMeshRenderer::Update()
	{
		if (!myModel)
			return;

		for (int i = 0; i < myModel->GetMeshes().size(); ++i)
		{
			MeshCommand command;
			command.myMesh = &myModel->GetMeshes()[i];
			command.myColor = myColor;
			command.myAdditiveColor = myAdditiveColor;
			command.myMatrix = GetTransform().GetMatrix();
			command.myMaterial = myMaterials[i];
			
			if (myAnimation)
			{
				command.myIsAnimated = true;
				for (auto entry : myAnimation->myBoneTransforms)
				{
					if (entry.first >= 128)
					{
						Console::ErrorLog("Too many bones in animation. (Max bones allowed: 128)");
						return;
					}

					command.myBoneTransforms[entry.first] = entry.second[1];
				}
			}

			Main::GetRenderManager().AddMesh(command);
		}
	}
}
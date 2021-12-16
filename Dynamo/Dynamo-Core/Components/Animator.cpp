#include "pch.h"
#include "Animator.h"
#include "Rendering/Model.h"
#include "Rendering/Animation.h"

namespace Dynamo
{
	Animator::Animator()
	{

	}

	void Animator::ExposeValues()
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100);
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
		ImGui::Text("FPS");
		ImGui::NextColumn();
		if (ImGui::InputInt("##animatorfps", &myFPS))
		{
			myAnimationProgress = 0;
		}

		ImGui::Columns(1);
	}

	nlohmann::json Animator::Save()
	{
		nlohmann::json json;

		json["Animation"] = "";
		if(myAnimation)
			json["Animation"] = myAnimation->myPath.string();

		json["fps"] = myFPS;
		return json;
	}

	void Animator::Load(nlohmann::json& aJson)
	{
		std::string animationPath = aJson["Animation"];
		if (animationPath != "")
			myAnimation = AnimationFactory::GetAnimation(animationPath);

		myFPS = aJson["fps"];
	}

	void Animator::OnCreate()
	{
		myMeshRenderer = GetComponent<MeshRenderer>();
	}

	void Animator::Update()
	{
		if (!myMeshRenderer)
		{
			myMeshRenderer = GetComponent<MeshRenderer>();
			return;
		}

		if (!myAnimation)
			return; 

		myAnimationProgress -= Time::GetDeltaTime();
		if (myAnimationProgress <= 0)
		{
			float animationDelay = 1 / (float)myFPS;
			myAnimationProgress += animationDelay;

			myCurrentFrameIndex++;
			if (myCurrentFrameIndex > myAnimation->myFrameCount)
			{
				myCurrentFrameIndex = 1;
			}
		}

		for (auto& entry : myAnimation->myBoneTransforms)
		{
			myBoneTransforms[entry.first] = entry.second[myCurrentFrameIndex];
		}

		myMeshRenderer->SetBoneTransforms(&myBoneTransforms);
	}

	void Animator::EditorUpdate()
	{
		Update();
	}
}
#include "pch.h"
#include "MeshRenderer.h"
#include "Factories/ModelFactory.h"
#include "Rendering/Model.h"
#include "Rendering/RenderManager.h"
#include "Rendering/Material.h"

namespace Dynamo
{
	void MeshRenderer::ExposeValues()
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 100);

		ImGui::Text("Model");
		ImGui::NextColumn();

		std::string buttonName = "Unassigned";
		if(myModel)
			buttonName = myModel->GetPath() + "##meshrenderermodel";

		ImGui::Button(buttonName.c_str(), ImVec2(340, 20));
		if (ImGui::IsItemHovered() && myModel)
		{
			ImGui::SetTooltip(myModel->GetPath().c_str());
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".fbx"))
			{
				std::string filePath = *(std::string*)payload->Data;
				SetModel(filePath);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::NextColumn();
		ImGui::Text("Color");
		ImGui::NextColumn();
		ImGui::ColorEdit3("##meshrenderercolor", &myColor.x);

		ImGui::Separator();

		for (int i = 0; i < myMaterials.size(); ++i)
		{
			ImGui::NextColumn();
			std::string matName = "Material " + std::to_string(i);
			ImGui::Text(matName.c_str());
			ImGui::NextColumn();
			std::string name = myMaterials[i]->myMaterialPath.string() + "##meshrenderermaterial";
			name += i;
			ImGui::Button(name.c_str(), ImVec2(340, 20));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".dynmaterial"))
				{
					std::string filePath = *(std::string*)payload->Data;
					myMaterials[i] = MaterialFactory::GetMaterial(filePath);
				}

				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Columns(1);
	}

	nlohmann::json MeshRenderer::Save()
	{
		nlohmann::json json;

		if (!myModel)
			return json;

		json["path"] = myModel->GetPath();
		json["color"]["r"] = myColor.r;
		json["color"]["g"] = myColor.g;
		json["color"]["b"] = myColor.b;
		json["color"]["a"] = myColor.a;

		for (int i = 0; i < myMaterials.size(); ++i)
		{
			json["materials"].push_back(myMaterials[i]->myMaterialPath.string());
		}

		return json;
	}

	void MeshRenderer::Load(nlohmann::json& aJson)
	{
		SetModel(aJson["path"]);
		ApplyModelMaterial();

		myColor.r = aJson["color"]["r"];
		myColor.g = aJson["color"]["g"];
		myColor.b = aJson["color"]["b"];
		myColor.a = aJson["color"]["a"];

		for (int i = 0; i < aJson["materials"].size(); ++i)
		{
			if (i > myMaterials.size())
			{
				Console::ErrorLog("Model & Scene mesh count missmatch");
				return;
			}

			std::string path = aJson["materials"][i];
			if(path != "")
				myMaterials[i] = MaterialFactory::GetMaterial(path);
		}
	}

	void MeshRenderer::Update()
	{
		if (!myModel)
			return;

		for(int i = 0; i < myModel->GetMeshes().size(); ++i)
		{
			MeshCommand command;
			command.myMesh = &myModel->GetMeshes()[i];
			command.myColor = myColor;
			command.myMatrix = GetTransform().GetMatrix();
			command.myMaterial = myMaterials[i];

			Main::GetRenderManager().AddMesh(command);
		}
	}

	void MeshRenderer::EditorUpdate()
	{
		Update();
	}

	void MeshRenderer::SetModel(const std::string& aPath)
	{
		myModel = ModelFactory::GetModel(aPath);
		ApplyModelMaterial();
	}

	void MeshRenderer::SetModel(Model* aModel)
	{
		myModel = aModel;
		ApplyModelMaterial();
	}

	const std::vector<Mesh>& MeshRenderer::GetMeshes() const
	{
		return myModel->GetMeshes();
	}
	
	void MeshRenderer::SetColor(const Color& aColor)
	{
		myColor = aColor;
	}

	const Color& MeshRenderer::GetColor() const
	{
		return myColor;
	}

	void MeshRenderer::ApplyModelMaterial()
	{
		myMaterials.clear();
		for (int i = 0; i < myModel->GetMeshes().size(); ++i)
		{
			std::filesystem::path path = myModel->GetPath();
			path.replace_extension(".dynmaterial");
			myMaterials.push_back(MaterialFactory::GetMaterial(path.string()));

			// Still works. Will apply materials to model using paths.
			//myMaterials.push_back(MaterialFactory::GetMaterialForModel(myModel->GetPath()));
		}
	}
	void MeshRenderer::SetMaterialOnAllMeshes(Material* aMat)
	{
		for (int i = 0; i < myMaterials.size(); ++i)
		{
			myMaterials[i] = aMat;
		}
	}
}
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
		ImGui::Text("Model");
		ImGui::SameLine(0, 40);

		std::string modelPath = "";
		if (myModel)
			modelPath = myModel->GetPath();

		ImGui::Button(modelPath.c_str(), ImVec2(500, 20));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(modelPath.c_str());
		}

		ImGui::Text("Color");
		ImGui::SameLine(0, 40);
		ImGui::ColorEdit3("##meshrenderercolor", &myColor.r);

		ImGui::Text("Materials:");
		for (int i = 0; i < myMaterials.size(); ++i)
		{
			std::string name = "Material " + i;
			name += "##meshrenderer";
			if(ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_Framed))
			{
				ImGui::Text("Roughness constant");
				ImGui::SameLine(0.0f, 75);
				ImGui::DragFloat("##meshrendererroughnessconstnat" + i, &myMaterials[i]->myRoughnessConstant, 0.01f, 0, 1);

				ImGui::Text("Roughness interpolation");
				if(ImGui::IsItemHovered())
					ImGui::SetTooltip("Interpolates between material texture and Roughness Constant \n A value of 1 = Material Texture\n A value of 0 = Roughness constant");
				ImGui::SameLine(0.0f, 40);
				ImGui::DragFloat("##meshrendererroughnessinterpolation" + i, &myMaterials[i]->myRoughnessInterpolation, 0.01f, 0, 1);

				ImGui::Text("Metalness constant");
				ImGui::SameLine(0.0f, 75);
				ImGui::DragFloat("##meshrenderermetalnessconstnat" + i, &myMaterials[i]->myMetalnessConstant, 0.01f, 0, 1);

				ImGui::Text("Metalness interpolation");
				if(ImGui::IsItemHovered())
					ImGui::SetTooltip("Interpolates between material texture and Metalness Constant \n A value of 1 = Material Texture\n A value of 0 = Metalness constant");
				ImGui::SameLine(0.0f, 40);
				ImGui::DragFloat("##meshrenderermetalnessinterpolation" + i, &myMaterials[i]->myMetalnessInterpolation, 0.01f, 0 ,1);
			}
		}
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
	}

	void MeshRenderer::Update()
	{
		for(const auto& mesh : myModel->GetMeshes())
		{
			MeshCommand command;
			command.myMesh = &mesh;
			command.myColor = myColor;
			if (!myMaterials.empty())
				command.myMaterial = myMaterials.front();
			
			command.myMatrix = GetTransform().GetMatrix();

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
		isInitialized = true;
	}

	void MeshRenderer::SetModel(Model* aModel)
	{
		myModel = aModel;
		isInitialized = true;
	}

	void MeshRenderer::SetMaterial(Material* aMaterial)
	{
		myMaterials.clear();
		myMaterials.push_back(aMaterial);
	}

	void MeshRenderer::AddMaterial(Material* aMaterial)
	{
		myMaterials.push_back(aMaterial);
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

	Material* MeshRenderer::GetMaterial() const
	{
		if (myMaterials.size() > 0)
			return myMaterials.front();

		return nullptr;
	}

	void MeshRenderer::ApplyModelMaterial()
	{
		AddMaterial(MaterialFactory::GetMaterialForModel(myModel->GetPath()));
	}

	bool MeshRenderer::IsInitialized() const
	{
		return isInitialized && myMaterials.size() > 0;
	}
}
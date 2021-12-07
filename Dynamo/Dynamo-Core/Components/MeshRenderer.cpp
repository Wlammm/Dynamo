#include "pch.h"
#include "MeshRenderer.h"
#include "Factories/ModelFactory.h"
#include "Rendering/Model.h"
#include "Rendering/RenderManager.h"

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

	void MeshRenderer::OnCreate()
	{
		Main::GetRenderManager().AddMesh(this);
	}

	void MeshRenderer::OnEnable()
	{
		Main::GetRenderManager().AddMesh(this);
	}

	void MeshRenderer::OnDestroy()
	{
		Main::GetRenderManager().RemoveMesh(this);
	}

	void MeshRenderer::OnDisable()
	{
		Main::GetRenderManager().RemoveMesh(this);
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
#include "pch.h"
#include "MeshRenderer.h"
#include "Factories/ModelFactory.h"
#include "Rendering/Model.h"
#include "Rendering/RenderManager.h"

namespace Dynamo
{
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
	}

	void MeshRenderer::SetModel(Model* aModel)
	{
		myModel = aModel;
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
}
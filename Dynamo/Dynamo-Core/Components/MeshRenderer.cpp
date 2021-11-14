#include "pch.h"
#include "MeshRenderer.h"
#include "Factories/ModelFactory.h"

namespace Dynamo
{
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
}
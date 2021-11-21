#include "pch.h"
#include "Model.h"

namespace Dynamo
{
	void Dynamo::Model::AddMesh(const Mesh& aMesh)
	{
		myMeshes.push_back(aMesh);
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return myMeshes;
	}

	const std::string& Model::GetPath() const
	{
		return myPath;
	}
}
#include "pch.h"
#include "RenderManager.h"

namespace Dynamo
{
	void Dynamo::RenderManager::AddMesh(MeshRenderer* aMeshRenderer)
	{
		myMeshes.Add(aMeshRenderer);
	}

	void RenderManager::RemoveMesh(MeshRenderer* aMeshRenderer)
	{
		myMeshes.TryRemove(aMeshRenderer);
	}

	void RenderManager::Render()
	{

	}
}

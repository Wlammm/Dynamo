#include "pch.h"
#include "RenderManager.h"
#include "ForwardRenderer.h"

namespace Dynamo
{
	RenderManager::RenderManager()
	{
	}
	
	RenderManager::~RenderManager()
	{
	}

	void RenderManager::AddMesh(MeshRenderer* aMeshRenderer)
	{
		myModels.Add(aMeshRenderer);
	}

	void RenderManager::RemoveMesh(MeshRenderer* aMeshRenderer)
	{
		myModels.TryRemove(aMeshRenderer);
	}

	void RenderManager::Render()
	{
		//myForwardRenderer.Render(myModels);
	}
}

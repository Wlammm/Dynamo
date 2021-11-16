#pragma once

#include "ForwardRenderer.h"

namespace Dynamo
{
	class MeshRenderer;

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void AddMesh(MeshRenderer* aMeshRenderer);
		void RemoveMesh(MeshRenderer* aMeshRenderer);

		void Render();

	private:
		CU::DArray<MeshRenderer*> myModels;

		ForwardRenderer myForwardRenderer;
	};
}
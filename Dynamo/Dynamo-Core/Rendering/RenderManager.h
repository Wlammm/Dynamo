#pragma once

namespace Dynamo
{
	class MeshRenderer;

	class RenderManager
	{
	public:
		static void AddMesh(MeshRenderer* aMeshRenderer);
		static void RemoveMesh(MeshRenderer* aMeshRenderer);

		static void Render();

	private:
		static CU::DArray<MeshRenderer*> myMeshes;

	};
}
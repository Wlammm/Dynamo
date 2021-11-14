#pragma once

#include "Mesh.h"

namespace Dynamo
{
	class Model
	{
	public:
		Model() = default;
		~Model() = default;

		void AddMesh(const Mesh& aMesh);

		const std::vector<Mesh>& GetMeshes() const;

	private:
		std::vector<Mesh> myMeshes;
	};
}
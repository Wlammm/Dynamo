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

		const std::string& GetPath() const;

	private:
		friend class ModelFactory;

		std::vector<Mesh> myMeshes;
		std::string myPath = "";
	};
}
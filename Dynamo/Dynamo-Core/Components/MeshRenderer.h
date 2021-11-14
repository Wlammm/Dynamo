#pragma once

namespace Dynamo
{
	class Model;
	class Material;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer() = default;
		~MeshRenderer() = default;

		void SetModel(const std::string& aPath);
		void SetModel(Model* aModel);

		void AddMaterial(Material* aMaterial);

	private:
		Model* myModel = nullptr;

		std::vector<Material*> myMaterials;
	};
}
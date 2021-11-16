#pragma once

namespace Dynamo
{
	class Model;
	struct Material;
	struct Mesh;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer() = default;
		~MeshRenderer() = default;

		void SetModel(const std::string& aPath);
		void SetModel(Model* aModel);

		void AddMaterial(Material* aMaterial);

		const std::vector<Mesh>& GetMeshes() const;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		Material* GetMaterial() const;

	private:
		Model* myModel = nullptr;
		Color myColor = { 1, 1, 1, 1 };

		std::vector<Material*> myMaterials;
	};
}
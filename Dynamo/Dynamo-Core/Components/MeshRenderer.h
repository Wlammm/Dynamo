#pragma once

#include "ComponentDeclaration.hpp"

namespace Dynamo
{
	class Model;
	struct Material;
	struct Mesh;

	class MeshRenderer : public Component
	{
		COMP(MeshRenderer);
	public:
		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;


		MeshRenderer() = default;
		~MeshRenderer() = default;

		void Update() override;
		void EditorUpdate() override;

		void SetModel(const std::string& aPath);
		void SetModel(Model* aModel);

		const std::vector<Mesh>& GetMeshes() const;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		void ApplyModelMaterial();

		void SetMaterialOnAllMeshes(Material* aMat);

	private:
		Model* myModel = nullptr;
		Color myColor = { 1, 1, 1, 1 };

		std::vector<Material*> myMaterials;
	};
}
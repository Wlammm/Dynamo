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
		MeshRenderer() = default;
		~MeshRenderer() = default;

		void OnCreate() override;
		void OnEnable() override;

		void OnDestroy() override;
		void OnDisable() override;

		void SetModel(const std::string& aPath);
		void SetModel(Model* aModel);

		void AddMaterial(Material* aMaterial);

		const std::vector<Mesh>& GetMeshes() const;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		Material* GetMaterial() const;

		void ApplyModelMaterial();

	private:
		Model* myModel = nullptr;
		Color myColor = { 1, 1, 1, 1 };

		std::vector<Material*> myMaterials;
	};
}
#pragma once

namespace Dynamo
{
	struct Material;

	class MaterialFactory
	{
	public:
		static Material* GetMaterial(const std::string& aPath);

		static Material* GetDefaultMaterial();
		static Material* GetMissingMaterial();

		static Material* GetMaterialForModel(const std::string& aModelPath);

		static void SaveMaterial(const Material* aMaterial);
		static Material* CreateMaterial(const std::filesystem::path& aPath);

	private:
		static void LoadMaterial(const std::string& aPath);
		static void LoadModelMaterial(const std::string& aPath);

	private:
		static std::unordered_map<std::string, Material> myMaterials;
		static std::unordered_map<std::string, Material> myModelMaterials;
	};
}
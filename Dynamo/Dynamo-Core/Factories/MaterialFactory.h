#pragma once

namespace Dynamo
{
	struct Material;

	class MaterialFactory
	{
	public:
		static Material* GetMaterial(const std::string& aPath);

		static Material* GetDefaultMaterial();

	private:
		static void LoadMaterial(const std::string& aPath);

	private:
		static std::unordered_map<std::string, Material> myMaterials;
	};
}
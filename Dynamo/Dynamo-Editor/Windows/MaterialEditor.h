#pragma once

namespace Editor
{
	class MaterialEditor : public EditorWindow
	{
	public:
		MaterialEditor();

		void Update() override;

		void SetSelectedMaterial(Dyn::Material* aMaterial);

	private:
		std::array<std::string, 2> mySurfaceTypes = { "Opaque", "Transparent" };

		Dyn::Material* myMaterial = nullptr;
	};
}
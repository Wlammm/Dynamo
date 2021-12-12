#pragma once

namespace Editor
{
	class MaterialEditor : public EditorWindow
	{
	public:
		MaterialEditor();
		~MaterialEditor();

		void Update() override;

		void SetSelectedMaterial(Dyn::Material* aMaterial);

	private:
		void Save();

	private:
		float mySaveDuration = 2;
		float mySaveProgress = 0;

		std::array<std::string, 2> mySurfaceTypes = { "Opaque", "Transparent" };

		Dyn::Material* myMaterial = nullptr;
	};
}
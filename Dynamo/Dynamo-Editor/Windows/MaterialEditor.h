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

		void DrawAlbedoTexture();
		void DrawNormalTexture();
		void DrawMaterialTexture();
		void DrawCustomPart();
		void DrawShaderPart();
		void DrawCustomMatrial();
		void DrawSurfaceTypes();
		void DrawMaterialSettings();

	private:
		float mySaveDuration = 2;
		float mySaveProgress = 0;

		std::array<std::string, 2> mySurfaceTypes = { "Opaque", "Transparent" };

		Dyn::Material* myMaterial = nullptr;
	};
}
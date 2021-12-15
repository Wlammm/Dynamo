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
		void DrawRenderers();
		void DrawMaterialSettings();

	private:
		float mySaveDuration = 2;
		float mySaveProgress = 0;

		std::array<std::string, 2> myRendererTypes = { "Deferred", "Forward" };

		Dyn::Material* myMaterial = nullptr;
	};
}
#pragma once

namespace Editor
{
	class EditorManager;
	class PostMaster;

	class MainSingleton
	{
	public:
		static void Init();
		static void Destroy();
		static void Update();

		static void SetSelectedGameObject(GameObject* aGameObject);
		static GameObject* GetSelectedGameObject();

		static PostMaster* GetPostMaster();

		static EditorManager* GetEditorManager();

	private:
		static void InitScenes();

	private:
		EditorManager* myEditorManager = nullptr;
		PostMaster* myPostMaster = nullptr;

		GameObject* mySelectedGameObject = nullptr;

	private:
		inline static MainSingleton* ourInstance = nullptr;
	};
}
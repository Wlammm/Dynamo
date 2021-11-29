#pragma once

namespace Editor
{
	class EditorWindow;
	class EditorSystem;
	class Scene;

	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager();

		void Update();

		template<typename T>
		T* GetWindow()
		{
			for (int i = 0; i < myWindows.sizeI(); ++i)
			{
				if (T* ptr = dynamic_cast<T*>(myWindows[i]))
				{
					return ptr;
				}
			}

			return nullptr;
		}
		EditorWindow* AddWindow(EditorWindow* aWindow);
		void RemoveWindow(EditorWindow* aWindow);

		template<typename T>
		T* GetSystem()
		{
			for (int i = 0; i < mySystems.sizeI(); ++i)
			{
				if (T* ptr = dynamic_cast<T*>(mySystems[i]))
				{
					return ptr;
				}
			}

			return nullptr;
		}
		EditorSystem* AddSystem(EditorSystem* aSystem);
		void RemoveSystem(EditorSystem* aSystem);

	private:
		void InitWindows();
		void InitSystems();

		void BeginImGuiDocking();

	private:
		CU::DArray<EditorWindow*> myWindows{};
		CU::DArray<EditorSystem*> mySystems{};

		bool myIsRunning = true;
		int myNextID = 0;
	};
}
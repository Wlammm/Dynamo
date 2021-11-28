#pragma once

namespace Editor
{
	class EditorWindow;
	class EditorSystem;

	class EditorManager
	{
	public:
		EditorManager() = default;
		~EditorManager();

		void Run();

		void Update();

		EditorWindow* AddWindow(EditorWindow* aWindow);
		void RemoveWindow(EditorWindow* aWindow);

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

		EditorSystem* AddSystem(EditorSystem* aSystem);
		void RemoveSystem(EditorSystem* aSystem);

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


	private:
		void InitWindows();

		void BeginImGuiDocking();

	private:
		bool myIsRunning = true;

		CU::DArray<EditorWindow*> myWindows{};
		CU::DArray<EditorSystem*> mySystems{};
		int myNextID = 0;
	};
}
#pragma once

namespace Editor
{
	class EditorWindow
	{
	public:
		EditorWindow(const std::string& aWindowName);
		virtual ~EditorWindow() {};

		virtual void Init();

		void DoUpdate();

	protected:
		virtual void Update() = 0;

		friend class EditorManager;
		std::string myWindowName;
		int myWindowID = -1;
	};
}
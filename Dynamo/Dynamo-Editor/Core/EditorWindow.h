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

		void SetFocus();

	protected:
		virtual void Update() = 0;

		friend class EditorManager;
		std::string myWindowName;
		bool myShouldSetFocus = false;
		int myWindowID = -1;
	};
}
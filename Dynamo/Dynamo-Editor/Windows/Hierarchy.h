#pragma once

namespace Editor
{
	class Hierarchy : public EditorWindow
	{
	public:
		Hierarchy();

		void Update() override;

	private:
		void UpdateHotkeys();
		void UpdatePopups();
		void CreateNewGameObject();
	};
}
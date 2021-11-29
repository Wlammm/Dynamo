#pragma once

namespace Editor
{
	class Toolbar : public EditorSystem
	{
	public:
		Toolbar();

		void Update() override;

	private:
		void ShowFiles();
		void ShowThemes();
	};
}
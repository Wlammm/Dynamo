#pragma once

namespace Editor
{
	class Inspector : public EditorWindow
	{
	public:
		Inspector();

		void Init() override;

		void Update() override;
		
	private:
		void AddComponentButton();

		void UpdateHotkeys();

		void InitCategorizedComponents();
		std::map<std::string, std::vector<Component*>> myCategorizedComponents;
	};
}
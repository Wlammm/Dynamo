#include "EditorPch.h"
#include "Toolbar.h"

#include "Utils/ThemeUtils.h"

namespace Editor
{
	Toolbar::Toolbar()
	{

	}

	void Toolbar::Update()
	{
		if (ImGui::BeginMenuBar())
		{
			ShowFiles();
			ShowThemes();
			ShowDemo();
			ImGui::EndMenuBar();
		}
	}

	void Toolbar::ShowFiles()
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
	}

	void Toolbar::ShowThemes()
	{
		if (ImGui::BeginMenu("Themes"))
		{
			if (ImGui::MenuItem("Default"))
			{
				Themes::ApplyDefaultTheme();
			}

			if (ImGui::MenuItem("Theme 1"))
			{
				Themes::ApplyTheme1();
			}

			ImGui::EndMenu();
		}
	}

	void Toolbar::ShowDemo()
	{
		static bool open = false;
		if (ImGui::MenuItem("ImGui Demo"))
		{
			open = !open;
		}

		if(open)
			ImGui::ShowDemoWindow();
	}
}

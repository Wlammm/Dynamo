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
		}
		ImGui::EndMenuBar();
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
}

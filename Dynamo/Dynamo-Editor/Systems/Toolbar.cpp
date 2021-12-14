#include "EditorPch.h"
#include "Toolbar.h"

#include "Utils/ThemeUtils.h"

#include "Windows/MaterialEditor.h"
#include "Windows/NodeEditor.h"
#include "Windows/RenderDebugger.h"

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
			ShowWindows();
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

	void Toolbar::ShowWindows()
	{
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Material Editor"))
			{
				Main::GetEditorManager()->AddWindow(new MaterialEditor());
			}

			if (ImGui::MenuItem("Node Editor"))
			{
				Main::GetEditorManager()->AddWindow(new NodeEditor());
			}

			if (ImGui::MenuItem("Render Debugger"))
			{
				Main::GetEditorManager()->AddWindow(new RenderDebugger());
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

#include "EditorPch.h"
#include "EditorWindow.h"

Editor::EditorWindow::EditorWindow(const std::string& aWindowName)
{
	myWindowName = aWindowName;
}

void Editor::EditorWindow::DoUpdate()
{
	ImGui::Begin(myWindowName.c_str());

	Update();

	ImGui::End();
}

void Editor::EditorWindow::Update() { }

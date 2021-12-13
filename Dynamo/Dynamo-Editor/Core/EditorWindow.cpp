#include "EditorPch.h"
#include "EditorWindow.h"

Editor::EditorWindow::EditorWindow(const std::string& aWindowName)
{
	myWindowName = aWindowName;
}

void Editor::EditorWindow::Init() { }

void Editor::EditorWindow::DoUpdate()
{
	ImGui::Begin(myWindowName.c_str());

	if (myShouldSetFocus)
	{
		myShouldSetFocus = false;
		ImGui::SetWindowFocus();
	}

	Update();

	ImGui::End();
}

void Editor::EditorWindow::SetFocus()
{
	myShouldSetFocus = true;
}

void Editor::EditorWindow::Update() { }

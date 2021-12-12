#include "EditorPch.h"
#include "NodeEditor.h"

namespace Editor
{
	NodeEditor::NodeEditor()
		: EditorWindow("Node Editor")
	{
		ImGui::NodeEditor::Config config;
		config.SettingsFile = "Assets/Editor/NodeEditor/NodeEditorSettings.json";
		myEditorContext = ImGui::NodeEditor::CreateEditor(&config);
	}

	void NodeEditor::Update()
	{
		ImGui::NodeEditor::SetCurrentEditor(myEditorContext);
		ImGui::NodeEditor::Begin("NodeEditor");


		ImGui::NodeEditor::End();
	}
}

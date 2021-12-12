#pragma once

namespace Editor
{
	class NodeEditor : public EditorWindow
	{
	public:
		NodeEditor();

		void Update() override;

	private:
		ImGui::NodeEditor::EditorContext* myEditorContext = nullptr;
	};
}
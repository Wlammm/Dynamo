#include "EditorPch.h"
#include "NodeEditor.h"

#include "NodeEditor/Manager/NodeEditorManager.h"

#include "NodeEditor/Manager/NodeRegistry.h"

namespace Editor
{
	NodeEditor::NodeEditor()
		: EditorWindow("Node Editor")
	{ 
		NodeEditorManager::Create();
	}

	NodeEditor::~NodeEditor()
	{
		NodeEditorManager::Destroy();
	}

	void NodeEditor::Init()
	{
		NodeEditorManager::Load();
	}

	void NodeEditor::Update()
	{
		NodeEditorManager::BeginFrame();
		NodeEditorManager::Update();
		NodeEditorManager::EndFrame();
	}
}
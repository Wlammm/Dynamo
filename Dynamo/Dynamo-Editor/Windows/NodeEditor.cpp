#include "EditorPch.h"
#include "NodeEditor.h"
#include "NodeEditor/Manager/GraphManager.h"

namespace Editor
{
	NodeEditor::NodeEditor()
		: EditorWindow("Node Editor")
	{ 
		myGraphManager = new GraphManager();
	}

	NodeEditor::~NodeEditor()
	{
		delete myGraphManager;
		myGraphManager = nullptr;
	}

	void NodeEditor::Init()
	{
		myGraphManager->Load();
	}

	void NodeEditor::Update()
	{
		myGraphManager->PreFrame(Time::GetUnscaledDeltaTime());
		myGraphManager->ConstructEditorTreeAndConnectLinks();
		myGraphManager->PostFram();
	}
}
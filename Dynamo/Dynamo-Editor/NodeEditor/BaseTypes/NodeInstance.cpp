#include "EditorPch.h"
#include "NodeInstance.h"

#include "NodeEditor/Nodes/BaseNode.h"

#include "NodeEditor/Manager/NodeEditorManager.h"
#include "NodeEditor/Manager/NodeRegistry.h"

#include "NodeEditor/BaseTypes/NodePin.h"

Editor::NodeInstance::NodeInstance(const int aBaseNodeID, const unsigned int aUniqueID)
{
	myNodeType = NodeEditorManager::GetNodeRegistry().GetNodeFromID(aBaseNodeID);
	myUID = aUniqueID;
}

void Editor::NodeInstance::Enter()
{
	int index = myNodeType->Enter(this);
	index;
}

const std::string& Editor::NodeInstance::GetNodeName() const
{
	return myNodeType->GetNodeName();
}

std::vector<Editor::NodePin>& Editor::NodeInstance::GetPins()
{
	return myNodeType->GetPins();
}

#include "EditorPch.h"
#include "AddNode.h"

Editor::AddNode::AddNode()
{
	CreatePin("In", PinType::In, PinData::None);
	CreatePin("Value 1", PinType::In, PinData::Float);
	CreatePin("Value 2", PinType::In, PinData::Float);
	CreatePin("Out", PinType::In, PinData::None);

	SetNodeName("Add");
}

int Editor::AddNode::Enter(NodeInstance* aNodeInstance)
{
	aNodeInstance;

	return -1;
}

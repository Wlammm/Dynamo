#include "EditorPch.h"
#include "SubNode.h"

Editor::SubNode::SubNode()
{
	CreatePin("In", PinType::In, PinData::None);
	CreatePin("Value 1", PinType::In, PinData::Float);
	CreatePin("Value 2", PinType::In, PinData::Float);
	CreatePin("Out", PinType::In, PinData::None);

	SetNodeName("Subtract");
}

int Editor::SubNode::Enter(NodeInstance* aNodeInstance)
{
	aNodeInstance;

	return -1;
}

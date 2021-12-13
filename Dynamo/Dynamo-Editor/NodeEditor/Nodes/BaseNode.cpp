#include "EditorPch.h"
#include "BaseNode.h"

#include "NodeEditor/BaseTypes/NodeInstance.h"

Editor::BaseNode::~BaseNode()
{
	myPins.clear();
}

int Editor::BaseNode::GetID() const
{
	return myID;
}

int Editor::BaseNode::Enter(NodeInstance* aNodeInstance)
{
	aNodeInstance;
	return -1;
}

int Editor::BaseNode::EnterWithPayload(NodeInstance* aNodeInstance, Payload& aPayload)
{
	aNodeInstance;
	aPayload;
	return -1;
}

void Editor::BaseNode::CreatePin(const std::string& aLabel, const PinType aType, const PinData aDataType, const bool isReadOnly)
{
	myPins.push_back({ aLabel, aType, aDataType, isReadOnly });
}

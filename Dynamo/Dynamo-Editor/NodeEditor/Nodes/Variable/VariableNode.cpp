#include "EditorPch.h"
#include "VariableNode.h"
#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/BaseTypes/GraphVariable.h"

SetNode::SetNode()
{
	myHiddenFromMenu = true;
	CreatePin("Exec", PinDirection::PinDirection_IN);
	CreatePin("Out", PinDirection::PinDirection_OUT);
	CreatePin("Value", PinDirection::PinDirection_IN, DataType::Variable);
	CreatePin("Result", PinDirection::PinDirection_OUT, DataType::Variable);
}

int SetNode::OnExec(GraphNodeInstance* aNodeInstance)
{
	DataType outType;
	DataPtr outPinData;
	size_t outDataSize = 0;

	GetPinData(aNodeInstance, 2, outType, outPinData, outDataSize);

	aNodeInstance->SetVariable(outType, outPinData);

	SetPinDataRaw(aNodeInstance, 3, outPinData.GetPtr<char>(), outDataSize);

	return 1;
}

std::string SetNode::GetInstanceName(const GraphNodeInstance* aNodeInstance) const
{
	return std::string("Set ") + aNodeInstance->GetVariable()->GetName();
}

GetNode::GetNode()
{
	myHiddenFromMenu = true;
	CreatePin("Value", PinDirection::PinDirection_OUT, DataType::Variable);
}

int GetNode::OnExec(GraphNodeInstance* aNodeInstance)
{
	DataType outType;
	DataPtr outPinData;

	aNodeInstance->GetVariable()->GetRaw(outType, outPinData);

	SetPinDataRaw(aNodeInstance, 0, outPinData.GetPtr<char>(), outPinData.GetSize());

	return -1;
}

std::string GetNode::GetInstanceName(const GraphNodeInstance* aNodeInstance) const
{
	return std::string("Get ") + aNodeInstance->GetVariable()->GetName();
}

#include "EditorPch.h"
#include "AddNode.h"

#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/BaseTypes/TemplateHelper.h"

AddNode::AddNode()
{
	CreatePin("IN", PinDirection::PinDirection_IN);
	CreatePin("A", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Result 1", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("Result 2", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("Result 3", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("B", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Result 4", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("C", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("OUT", PinDirection::PinDirection_OUT);
}

int AddNode::OnExec(GraphNodeInstance* aNodeInstance)
{
	// Get data on pin index 0, this index is relative to what you push in the constructor
	const float input1 = TemplateHelper::GetPinData<float>(aNodeInstance, 0);

	// Get data on pin index 1, this index is relative to what you push in the constructor
	const float input2 = TemplateHelper::GetPinData<float>(aNodeInstance, 1);

	// Perform the Add operation we're supposed to do.
	const float result = input1 + input2;

	// Set the result we calculated to the data property on the Result pin.
	TemplateHelper::SetPinData(aNodeInstance, 2, result);

	return -1; // If the node doesn't have a flow, return -1 see the print node for reference
}

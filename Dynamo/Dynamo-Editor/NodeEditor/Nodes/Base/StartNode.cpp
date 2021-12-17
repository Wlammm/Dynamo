#include "EditorPch.h"
#include "StartNode.h"

#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/BaseTypes/TemplateHelper.h"

StartNode::StartNode()
{
	CreatePin("", PinDirection::PinDirection_OUT);
	CreatePin("Updating", PinDirection::PinDirection_IN, DataType::Bool);
}

int StartNode::OnExec(GraphNodeInstance* aNodeInstance)
{
	const bool shouldRepeat = TemplateHelper::GetPinData<bool>(aNodeInstance, 1);
	aNodeInstance->SetShouldTriggerAgain(shouldRepeat);
	return 0;
}

#include "EditorPch.h"
#include "PrintNode.h"

#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/BaseTypes/TemplateHelper.h"

namespace Editor
{
	PrintNode::PrintNode()
	{
		CreatePin("In", PinDirection::PinDirection_IN);
		CreatePin("Text", PinDirection::PinDirection_IN, DataType::String);
		CreatePin("Out", PinDirection::PinDirection_OUT);
	}

	int PrintNode::OnExec(GraphNodeInstance* aNodeInstance)
	{
		DataType outType;
		DataPtr outPinData;
		size_t outDataSize = 0;

		GetPinData(aNodeInstance, 1, outType, outPinData, outDataSize);

		if (outDataSize > 0)
		{
			if (outType == DataType::Int)
			{
				const int i = outPinData.Get<int>();
				Console::Log("%d", i);
			}
			else if (outType == DataType::String)
			{
				const std::string string = outPinData.Get<std::string>();
				Console::Log("%s", string.c_str());
			}
			else if (outType == DataType::Bool)
			{
				const bool b = outPinData.Get<bool>();
				const std::string text = b ? "True" : "False";
				Console::Log("%s", text.c_str());
			}
			else if (outType == DataType::Float)
			{
				const float f = outPinData.Get<float>();
				Console::Log("%f", f);
			}
		}
		return 2;
	}
}
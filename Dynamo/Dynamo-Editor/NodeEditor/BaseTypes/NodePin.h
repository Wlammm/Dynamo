#pragma once
#include "NodeTypes.h"

namespace Editor
{
	class GraphNodeInstance;

	struct NodePin
	{
		NodePin() = delete;
		NodePin(const std::string& aLabel, const PinType aType, const PinData aDataType, bool isReadOnly);
		~NodePin() = default;


		std::string myLabel = "undefined";
		PinData myPinData = PinData::Unknown;
		PinType myPinType;
		unsigned int myUID = -1;

		GraphNodeInstance* myParent;

		bool myIsReadOnly = false;
	};
}

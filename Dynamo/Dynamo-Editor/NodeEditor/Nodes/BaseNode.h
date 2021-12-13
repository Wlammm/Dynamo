#pragma once
#include "NodeEditor/BaseTypes/NodePin.h"

namespace Editor
{
	class Payload;
	class NodeInstance;

	class BaseNode
	{
		friend class NodeRegistry;

	public:
		BaseNode() = default;
		virtual ~BaseNode();

		int GetID() const;

		__forceinline const std::string& GetNodeName() const { return myNodeName; };

		virtual int Enter(NodeInstance* aNodeInstance);
		virtual int EnterWithPayload(NodeInstance* aNodeInstance, Payload& aPayload);

		std::vector<NodePin>& GetPins() { return myPins; };

	protected:
		void CreatePin(const std::string& aLabel, const PinType aType, const PinData aDataType = PinData::None, const bool isReadOnly = false);
		const void SetNodeName(const std::string& aNodeName) { myNodeName = aNodeName; };

	private:
		int myID = -1;
		std::vector<NodePin> myPins;
		std::string myNodeName = "undefined";

	};
}


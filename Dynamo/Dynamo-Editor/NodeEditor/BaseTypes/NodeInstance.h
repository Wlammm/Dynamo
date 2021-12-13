#pragma once
#include "NodeEditor/BaseTypes/NodePin.h"

namespace Editor
{
	class BaseNode;

	class NodeInstance
	{
	public:
		NodeInstance() = delete;
		NodeInstance(const int aBaseNodeID, const unsigned int aUniqueID);
		~NodeInstance() = default;

		void Enter();

		const ImVec2& GetPosition() const { return myPositionOnGraph; };
		const unsigned int GetUID() const { return myUID; };
		const std::string& GetNodeName() const;
		std::vector<NodePin>& GetPins();

	private:
		BaseNode* myNodeType;
		ImVec2 myPositionOnGraph = ImVec2(0, 0);

		unsigned int myUID = -1;

	};
}

